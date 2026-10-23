/*
 * A039425  Numbers whose base-10 representation has the same nonzero number
 *          of 4's and 7's.
 * https://oeis.org/A039425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search for integers whose decimal representation
   has the same non‑zero number of digits 4 and 7.  This implements OEIS A039425
   with 1‑based indexing (a(1) = 47). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of already found terms. */
    static seq_t *cache  = NULL;
    static size_t cache_size = 0;   /* number of terms currently stored */
    static size_t cache_cap   = 0;   /* allocated capacity */
    static idx_t  next_candidate = 1; /* integer to test next */

    if (n == 0)
        return 0; /* offset is 1; value for n==0 is irrelevant */

    /* Generate terms until we have at least n cached. */
    while (cache_size < n) {
        /* Find the next integer whose number of 4’s equals the number of
           7’s and both counts are non‑zero. */
        while (1) {
            idx_t tmp = next_candidate;
            int cnt4 = 0, cnt7 = 0;

            while (tmp > 0) {
                int d = (int)(tmp % 10);
                if (d == 4)
                    ++cnt4;
                else if (d == 7)
                    ++cnt7;
                tmp /= 10;
            }

            if (cnt4 == cnt7 && cnt4 > 0)
                break;  /* candidate found */
            ++next_candidate;
        }

        /* Store the found number in the cache. */
        seq_t term = (seq_t)next_candidate;
        ++next_candidate;  /* advance candidate for next search */

        /* Grow the cache if needed. */
        if (cache_cap == 0) {
            cache_cap = 16;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache)
                return 0;  /* allocation failure */
        } else if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return 0;  /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_size++] = term;
    }

    return cache[n - 1];
}
