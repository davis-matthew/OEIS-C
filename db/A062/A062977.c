/*
 * A062977  Difference between largest and smallest positive exponent in prime
 *          factorization of n; a(1) = 0 by convention.
 * https://oeis.org/A062977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max_{p|n} v_p(n) – min_{p|n} v_p(n), a(1)=0
 * Range:     u8 n <= 8704, u16 n <= 8704, u32 n <= 8704, u64 n <= 8704,
 *            u128 n <= 8704, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* computes the difference between the largest and smallest positive
   exponents in the prime factorization of n */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(1) = 0 by convention */
    if (n <= 1) {
        return (seq_t)0;
    }

    uint64_t x = (uint64_t)n;          /* work on a native 64‑bit value */
    uint64_t max_e = 0;
    uint64_t min_e = (uint64_t)-1;    /* large initial value */

    /* trial division: find all prime factors and their exponents */
    for (uint64_t p = 2; p <= x / p; ++p) {
        if (x % p == 0) {
            uint64_t cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
            }
            if (cnt > max_e) max_e = cnt;
            if (cnt < min_e) min_e = cnt;
        }
    }

    /* if x > 1, it is a prime factor with exponent 1 */
    if (x > 1) {
        if (1 > max_e) max_e = 1;
        if (1 < min_e) min_e = 1;
    }

    /* the difference fits safely into seq_t (it is < 64) */
    return (seq_t)(max_e - min_e);
}
