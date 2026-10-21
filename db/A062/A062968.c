/*
 * A062968  a(n) = n + 1 - d(n), where d(n) is the number of divisors
 *          function.
 * https://oeis.org/A062968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 1 - d(n) where d(n) is the number of divisors of n
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* a(1) is the first term defined in OEIS; values for n == 0 are
       undefined but harmless, return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Compute the number of divisors of n using trial division.  The
       result never exceeds 64 bits for the ranges considered. */
    uint64_t d = 1;                     /* start with product of (exp+1) */
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            d *= (exp + 1);
        }
    }
    if (m > 1)        /* remaining prime factor */
        d *= 2u;

    /* a(n) = n + 1 - d(n); compute in 64‑bit to avoid overflow, then
       cast to seq_t. */
    uint64_t result = (uint64_t)n + 1u - d;
    return (seq_t)result;
}
