/*
 * A211270  Number of integer pairs (x,y) such that 0 < x <= y <= n and x*y =
 *          2n.
 * https://oeis.org/A211270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((d(2*n) - 1)/2)
 * Range:     u8 n <= 7703, u16 n <= 7703, u32 n <= 7703, u64 n <= 7703,
 *            u128 n <= 7703, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*        where d(m) is the number of positive divisors of m */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 0 < n is assumed; if n < 1 the formula would give an undefined
       result, but we simply return 0 in that case. */
    if (n < 1)
        return (seq_t)0;

    /* Compute m = 2*n using a native 64‑bit type. */
    uint64_t m = (uint64_t)n * 2ULL;

    /* Factor m, counting exponents of prime factors. */
    uint64_t temp = m;
    seq_t divisors = 1;            /* product of (exponent+1) */

    for (uint64_t p = 2; p * p <= temp; ++p) {
        uint64_t exp = 0;
        while (temp % p == 0) {
            ++exp;
            temp /= p;
        }
        if (exp)
            divisors *= (exp + 1);
    }
    if (temp > 1)                 /* remaining prime factor > sqrt(m) */
        divisors *= 2;            /* exponent 1 --> (1+1) */

    /* floor((d(2*n) - 1)/2) */
    return (divisors - 1) / 2;
}
