/*
 * A201734  Numbers n such that 90*n + 47 is prime.
 * https://oeis.org/A201734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 112, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (A142313(n)-47)/90
 * where A142313(n) is the n-th prime congruent to 7 modulo 10
 * and having digital root 2.  Equivalently, a(n) is the n-th
 * integer k (starting with k=0) such that 90*k + 47 is prime.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2U)          return false;
    if (n % 2U == 0U)    return n == 2U;
    if (n % 3U == 0U)    return n == 3U;

    for (uint64_t i = 5ULL, w = 2ULL;
         i * i <= n; i += w, w = 6ULL - w)
    {
        if (n % i == 0ULL)
            return false;
    }
    return true;
}

/* Return the n-th term of the sequence (offset 1).  n == 0 is undefined. */
seq_t a(idx_t n)
{
    /* Sequential search for the n-th k with 90*k + 47 prime. */
    idx_t count = 0;
    idx_t k = 0;

    /* Loop until we have found the n-th qualifying k. */
    while (count < n)
    {
        /* Compute 90*k + 47 using 128‑bit arithmetic to avoid overflow. */
        uint64_t cand = (unsigned __int128)90U * k + 47U;

        if (is_prime_uint64(cand))
        {
            ++count;
            if (count == n)
                return (seq_t)k;   /* k fits in seq_t for all tested n. */
        }
        ++k;
    }
    /* Should never reach here for valid input. */
    return 0;
}
