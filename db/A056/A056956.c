/*
 * A056956  Numbers n such that 6n+1 and 6n+5 are both primes.
 * https://oeis.org/A056956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k such that 6k+1 and 6k+5 are
 *            both primes
 * Range:     u8 n <= 15, u16 n <= 923, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   ensure_capacity, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: incremental search with memoization and a primality test. */
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* Offset is 1; no values requested for n == 0. */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t found = 0;          /* number of terms stored in cache */
    static idx_t last_candidate = 1; /* next integer to test */

    /* Grow cache to accommodate n terms. */
    if (n > cap) {
        ensure_capacity((size_t)n, &cap, (void **)&cache, sizeof(*cache));
    }

    /* Generate new terms until we have at least n. */
    while (found < n) {
        seq_t p1 = (seq_t)6 * last_candidate + 1;
        seq_t p5 = p1 + 4; /* 6k+5 */

        if (isPrimeGeneric(p1) && isPrimeGeneric(p5)) {
            /* Store the candidate. */
            cache[found] = last_candidate;
            ++found;
        }
        ++last_candidate;
    }

    return cache[n - 1];
}
