/*
 * A045861  Numbers whose square has initial digit '7'.
 * https://oeis.org/A045861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 4403, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045861: numbers whose square has initial decimal digit 7.
   The term a(n) is the n‑th integer k ≥ 1 such that the most significant
   decimal digit of k² is 7. The implementation searches numbers in
   increasing order and caches the results. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 0‑offset guard – the problem defines a(1) as the first term. */
    if (n == 0) return 0;

    /* static cache of computed terms and bookkeeping of its size */
    static seq_t *cache          = NULL; /* array of terms */
    static size_t cache_size    = 0;     /* number of terms stored */
    static size_t cache_cap     = 0;     /* allocated capacity of the array */

    /* next integer to test – starts at 1 and grows always by 1 */
    static seq_t candidate_next = 1;

    /* generate terms until we have at least n of them */
    while (cache_size < n) {
        seq_t cand = candidate_next++;       /* next candidate integer */
        seq_t sq   = cand * cand;             /* its square                */

        /* find the most significant decimal digit of sq */
        seq_t msd = sq;
        while (msd >= 10) msd /= 10;

        if (msd == 7) {                      /* qualifies as a term      */
            /* grow cache if needed */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc((void *)cache,
                                               new_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failure – abort */
                    return 0;
                }
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_size++] = cand;       /* store the term            */
        }
    }

    /* 1‑based indexing for the sequence */
    return cache[n - 1];
}
