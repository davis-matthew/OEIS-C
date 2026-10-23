/*
 * A195100  Numbers n such that there are no primes between (n-1)*sqrt(n-1)
 *          and n*sqrt(n).
 * https://oeis.org/A195100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 15, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A195100: Numbers n such that no prime lies between (n-1)*sqrt(n-1)
 * and n*sqrt(n).  The n‑th term a(n) is the n‑th integer that satisfies
 * this property.  The algorithm computes the interval borders as
 * floor_sqrt((n-1)^3)+1 and floor_sqrt(n^3) and tests the interval
 * for a prime.  Results are memoised for fast repeated calls. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime_big.h"

/* Cached sequence values (allow up to 20 000 terms, plenty for n ≤ 10 000). */
#define CACHE_CAP 20000
static seq_t cache[CACHE_CAP];
static idx_t cache_len = 0;          /* number of terms stored */

/* Current integer being inspected as a candidate. */
static idx_t current_candidate = 1;
/* floor_sqrt((candidate-1)^3) – used to compute the low bound of the next
 * candidate’s interval.  At start it is 0, i.e. floor_sqrt(0). */
static seq_t prev_high = 0;

/* Return true iff there exists a prime p such that low ≤ p ≤ high. */
static bool prime_in_interval(seq_t low, seq_t high)
{
    /* Scan the (small) interval directly; the length is < 200 for all
     * relevant n, so this is inexpensive. */
    for (seq_t x = low; x <= high; ++x)
        if (is_prime_big(x))
            return true;
    return false;
}

/* OEIS A195100 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* the offset is 1; a(0) is unused */

    /* Grow the cache until it contains the required term. */
    while (cache_len < n) {
        seq_t cand = (seq_t)current_candidate;

        /* Compute the interval bounds for the current candidate. */
        seq_t cube  = cand * cand * cand;          /* cand³   */
        seq_t high  = floor_sqrt(cube);           /* floor(√cand³) */
        seq_t low   = prev_high + 1;              /* floor(√(cand-1)³)+1 */

        /* Accept the candidate if the interval contains no prime. */
        if (!prime_in_interval(low, high))
            cache[cache_len++] = cand;

        /* Update state for the next candidate. */
        prev_high = high;
        ++current_candidate;
    }
    return cache[n - 1];
}
