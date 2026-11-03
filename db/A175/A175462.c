/*
 * A175462  Number of divisors of integers of the form 5 + 8n.
 * https://oeis.org/A175462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d(5 + 8*n) (number of divisors of 5 + 8*n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 119 terms through n = 9889 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper function that returns the divisor count of a 64‑bit unsigned integer.
   It is safe for all 0 < n < 2^64. */
static seq_t divisor_count_ull(uint64_t n)
{
    if (n == 0) return 0;
    seq_t count = 1;
    for (uint64_t p = 2; p <= n / p; ++p) {
        if (n % p == 0) {
            unsigned exp = 0;
            do {
                n /= p;
                ++exp;
            } while (n % p == 0);
            count *= (exp + 1);
        }
    }
    if (n > 1) {
        count *= 2;   /* remaining prime factor */
    }
    return count;
}

seq_t a(idx_t n)
{
    /* Compute 5 + 8*n using 64‑bit arithmetic.  For n within reasonable
       bounds this will not overflow. */
    uint64_t m = 5ull + 8ull * (uint64_t)n;
    return divisor_count_ull(m);
}
