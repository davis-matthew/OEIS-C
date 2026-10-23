/*
 * A107306  Numbers k such that (17*k - 19) is prime.
 * https://oeis.org/A107306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the k such that 17*k‑19 is prime; found by scanning
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Deterministic primality test for 64‑bit values – fast enough for the
   numbers 17*k‑19 that occur in this sequence (they are less than a few
   millions for the ranges tested). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2ULL)            return false;
    if (n % 2ULL == 0ULL)    return n == 2ULL;
    if (n % 3ULL == 0ULL)    return n == 3ULL;
    for (uint64_t i = 5ULL; i <= n / i; i += 6ULL) {
        if (n % i == 0ULL || n % (i + 2ULL) == 0ULL)
            return false;
    }
    return true;
}

/* Return the n-th term of A107306.  n is 1‑based. */
seq_t a(idx_t n)
{
    uint64_t k = 0ULL;        /* candidate k */
    uint64_t found = 0ULL;    /* how many qualifying k's found */

    while (found < n) {
        ++k;
        uint64_t val = 17ULL * k - 19ULL;
        if (is_prime_uint64(val))
            ++found;
    }

    return (seq_t)k;
}
