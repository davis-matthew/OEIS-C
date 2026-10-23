/*
 * A045752  4n-1 is composite.
 * https://oeis.org/A045752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 167, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045752:  a(n) = the n-th integer k ≥ 1 such that 4·k−1 is composite. */
/* The function caches results in static storage and generates new terms on demand. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Simple deterministic test for primes up to 2^64 (sufficient for the values needed). */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* Return the n‑th term of A045752 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n < 1 */

    /* Static cache of computed terms (stored as uint64_t to avoid overflow of seq_t). */
    static uint64_t *cache = NULL;       /* array of cached k values */
    static size_t used   = 0;            /* number of values already stored */
    static size_t cap    = 0;            /* current allocation size */
    static uint64_t cand  = 1;           /* next integer k to test */

    /* Grow cache if needed. */
    if ((size_t)n > cap) {
        size_t new_cap = (cap > 0) ? (cap << 1) : 32;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        uint64_t *tmp = (uint64_t *)realloc(cache, new_cap * sizeof(uint64_t));
        if (!tmp) return 0;            /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Generate missing terms. */
    while (used < (size_t)n) {
        uint64_t val = 4 * cand - 1;      /* test number 4·k−1 */
        if (!is_prime_u64(val))
            cache[used++] = cand;        /* composite ⇒ k belongs to sequence */
        cand++;                          /* next candidate */
    }

    return (seq_t)cache[(size_t)n - 1];
}
