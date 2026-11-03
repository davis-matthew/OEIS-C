/*
 * A078316  Maximum exponent in the prime factorization of n*rad(n)+1, where
 *          rad = A007947 (squarefree kernel).
 * https://oeis.org/A078316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum exponent in the prime factorization of
 *            n*rad(n)+1, where rad(n) = product of distinct primes dividing
 *            n. The function uses simple trial division and works for all n
 *            >= 1 that fit within the limits of a 64‑bit integer. For larger
 *            builds the seq_t type is assumed to hold arbitrary pr...
 * Range:     u8 n <= 22, u16 n <= 847, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9942 (2026-11-01)
 * Helpers:   radical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/radical.h"

/* Maximum exponents are small, so ptr type seq_t is sufficient. */
static inline seq_t factor_max_exponent(seq_t value)
{
    seq_t max_exp = 0;
    if (value <= 1) return 0;

    /* Factor out 2 */
    seq_t exp = 0;
    while ((value & 1) == 0) {
        value >>= 1;
        exp++;
    }
    if (exp > max_exp) max_exp = exp;

    /* Odd factors */
    for (seq_t p = 3; (p * p) <= value; p += 2) {
        exp = 0;
        while (value % p == 0) {
            value /= p;
            exp++;
        }
        if (exp > max_exp) max_exp = exp;
    }
    /* Remaining prime */
    if (value > 1 && 1 > max_exp) max_exp = 1;
    return max_exp;
}

seq_t a(idx_t n)
{
    /* Rad(n) */
    seq_t rad = radical(n);
    /* Compute m = n*rad + 1  (use seq_t for potential overflow) */
    seq_t m = (seq_t)n * rad + 1;
    /* Find maximum exponent in factorization of m */
    return factor_max_exponent(m);
}
