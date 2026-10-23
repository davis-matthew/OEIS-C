/*
 * A107331  An approximation to sigma_{1/2}(n): multiplicative with a(p^e) =
 *          floor((p^(e/2+1/2)-1)/(p^(1/2)-1)) for prime p.
 * https://oeis.org/A107331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 1798, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = multiplicative with a(p^e) = floor((p^((e+1)/2)-1)/(p^(1/2)-1))
 * Computation of a(p^e) uses integer arithmetic only.
 */
#include <stdint.h>
#include "../utils/bitlen_seq.h"

/* integer square root (floor) for an arbitrary-precision seq_t */
static inline seq_t floor_sqrt_seq(seq_t x)
{
    if (x == (seq_t)0)
        return (seq_t)0;

    idx_t bits = bitlen_seq(x);
    seq_t low  = (seq_t)0;
    seq_t high = ((seq_t)1) << ((bits + 1) >> 1);

    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        seq_t sq  = mid * mid;
        if (sq <= x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* compute a(p^e) for prime p and exponent e >= 1 */
static inline seq_t prime_power_a(uint64_t p, idx_t e)
{
    if ((e & 1u) == 1u) {                /* e odd: e = 2m-1,  m >= 1 */
        idx_t m = (e + 1) >> 1;          /* m = (e+1)/2 */
        seq_t T = (seq_t)1;              /* sum_{k=0}^{m-1} p^k */
        seq_t pow = (seq_t)1;
        for (idx_t i = 1; i < m; ++i) {
            pow *= (seq_t)p;
            T += pow;
        }
        seq_t inside = (seq_t)p * T * T;             /* p * T^2 */
        seq_t fs = floor_sqrt_seq(inside);          /* floor(T * sqrt(p)) */
        return T + fs;
    } else {                                   /* e even: e = 2m, m >= 1 */
        idx_t m = e >> 1;                       /* m = e/2 */
        seq_t U = (seq_t)1;                     /* sum_{k=0}^{m-1} p^k */
        seq_t pow = (seq_t)1;
        for (idx_t i = 1; i < m; ++i) {
            pow *= (seq_t)p;
            U += pow;
        }
        seq_t pow_m = pow * (seq_t)p;            /* p^m */
        seq_t S = U + pow_m;                     /* sum_{k=0}^{m} p^k */
        seq_t inside = (seq_t)p * U * U;          /* p * U^2 */
        seq_t fs = floor_sqrt_seq(inside);       /* floor(U * sqrt(p)) */
        return S + fs;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t tmp = (uint64_t)n;
    seq_t res = (seq_t)1;

    /* factor 2 separately */
    if ((tmp & 1ULL) == 0) {
        idx_t e = 0;
        while ((tmp & 1ULL) == 0) {
            tmp >>= 1;
            ++e;
        }
        res *= prime_power_a(2ULL, e);
    }

    /* odd prime factors */
    for (uint64_t p = 3ULL; p * p <= tmp; p += 2ULL) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            res *= prime_power_a(p, e);
        }
    }

    /* remaining prime > sqrt */
    if (tmp > 1ULL)
        res *= prime_power_a(tmp, 1);

    return res;
}
