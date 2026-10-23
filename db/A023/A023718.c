/*
 * A023718  Numbers with a single 3 in their base 4 expansion.
 * https://oeis.org/A023718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th positive integer whose base‑4 representation
 *            contains exactly one digit 3
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation lazily builds the sequence.  A static array holds
   all terms that have been generated so far.  Each call expands the
   array only when a larger index is required.  The algorithm is
   essentially O(n) for the n‑th term and stays well under two seconds
   for n ≈ 10 000. */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;    /* undefined for offset 1 */

    /* static cache of computed terms */
    static seq_t *cache = NULL;     /* array of terms */
    static idx_t cache_size = 0;    /* number of terms stored */
    static idx_t cache_cap  = 0;    /* allocated capacity */

    /* grow the cache until it contains at least n terms */
    while (cache_size < n) {
        /* find the next integer with exactly one 3 in base‑4 */
        static seq_t candidate = 1; /* remember last tested number */
        while (1) {
            seq_t x = candidate;
            seq_t tmp = x;
            int count3 = 0;
            /* count digits equal to 3 in base‑4 */
            while (tmp != (seq_t)0) {
                if ((tmp % (seq_t)4) == (seq_t)3) {
                    ++count3;
                    if (count3 > 1) break;
                }
                tmp /= (seq_t)4;
            }
            ++candidate; /* advance for next search */

            if (count3 == 1) {
                /* enlarge array if necessary */
                if (cache_size == cache_cap) {
                    idx_t new_cap = cache_cap ? cache_cap * 2U : 16U;
                    seq_t *tmp_cache = (seq_t *)realloc(cache,
                                                new_cap * sizeof(seq_t));
                    if (tmp_cache == NULL) abort(); /* allocation failure */
                    cache = tmp_cache;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = x; /* store found term */
                break;                   /* move on to next required term */
            }
        }
    }
    return cache[n-1]; /* 1‑based indexing */
}
