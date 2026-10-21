/*
 * A216325  Number of divisors of the degree of the minimal polynomial for
 *          2*cos(Pi/n), n >= 1.
 * https://oeis.org/A216325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(δ(n)), n≥1 where δ(n) = φ(2n)/2 (δ(1)=1), τ is number
 *            of divisors
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t phi_uint64(uint64_t m)
{
    uint64_t res = m;
    uint64_t tmp = m;
    if (tmp % 2 == 0) {
        while (tmp % 2 == 0) tmp /= 2;
        res -= res / 2;
    }
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            res -= res / p;
        }
    }
    if (tmp > 1) res -= res / tmp;
    return res;
}

static uint64_t divisor_count(uint64_t x)
{
    uint64_t res = 1;
    uint64_t tmp = x;
    if (tmp % 2 == 0) {
        int e = 0;
        while (tmp % 2 == 0) {
            tmp /= 2;
            e++;
        }
        res *= (uint64_t)(e + 1);
    }
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            res *= (uint64_t)(e + 1);
        }
    }
    if (tmp > 1) res *= 2ULL;
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    /* compute φ(2n) */
    uint64_t m = (uint64_t)n * 2ULL;
    uint64_t phi_val = phi_uint64(m);
    uint64_t delta = phi_val / 2ULL;               /* δ(n) */
    uint64_t tau = divisor_count(delta);           /* τ(δ(n)) */
    return (seq_t)tau;
}
