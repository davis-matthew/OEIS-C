/*
 * A065741  Largest square <= sum of squares of divisors of n.
 * https://oeis.org/A065741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor( sqrt( Σ_{d|n} d² ) )²
 * Range:     u8 n <= 14, u16 n <= 215, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 983 (2026-11-01)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the largest square ≤ the sum of squares of divisors of n. */

#include <stdint.h>
#include "../utils/isqrt_seq.h"

/* Compute σ₂(n) = Σ_{d|n} d² */
static seq_t sigma2_divisor_square_sum(idx_t n)
{
    seq_t result = 1;          /* multiplicative identity */
    idx_t tmp = n;
    idx_t p = 2;

    /* Handle the factor 2 separately for speed */
    if (tmp % p == 0) {
        int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        seq_t sum = 1;         /* 1 + p² + … + p^(2e) */
        seq_t pow = 1;         /* p^i */
        for (int i = 0; i < e; ++i) {
            pow *= p;                 /* pow = p^i */
            sum += pow * pow;         /* add p^(2i) */
        }
        result *= sum;
    }

    /* Odd prime factors */
    p = 3;
    while (p <= tmp / p) {          /* while p·p ≤ tmp, avoid overflow */
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            seq_t sum = 1;
            seq_t pow = 1;
            for (int i = 0; i < e; ++i) {
                pow *= p;
                sum += pow * pow;
            }
            result *= sum;
        }
        p += 2;                      /* next odd candidate */
    }

    /* Remaining prime greater than √n */
    if (tmp > 1) {
        /* exponent e = 1 for this prime */
        seq_t sum = 1 + tmp * tmp;   /* 1 + p² */
        result *= sum;
    }

    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset 1; behaviour for n<1 is unused */
    seq_t sig2 = sigma2_divisor_square_sum(n);
    seq_t r = isqrt_seq(sig2);
    return r * r;
}
