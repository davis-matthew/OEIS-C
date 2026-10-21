/*
 * A088230  a(1)=0; for n>1: a(n)=a(n-1)-n if that number is nonnegative and
 *          occurs at most once in the sequence, otherwise a(n)=a(n-1)+n.
 * https://oeis.org/A088230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 181, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A088230:  a(1)=0; for n>1: a(n)=a(n-1)-n if that number
   is nonnegative and occurs at most once in the sequence,
   otherwise a(n)=a(n-1)+n. */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms: cache[i] == a(i+1) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* allocated capacity */
    static size_t size  = 0;      /* number of terms stored */

    if (n == 0) return 0;  /* safety guard – n>=1 by definition */

    /* extend cache if needed */
    if (n > size) {
        size_t new_cap = n;
        if (cache == NULL) {
            cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
            if (cache == NULL) return 0;            /* allocation failure */
            cap = new_cap;
            /* initialise first term a(1) = 0 */
            cache[0] = 0;
            size = 1;
        } else if (new_cap > cap) {
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0;            /* allocation failure */
            cache = tmp;
            cap = new_cap;
        }

        /* compute terms from (size+1) to n */
        for (idx_t i = size + 1; i <= n; ++i) {
            seq_t step = (seq_t)(i - 1);          /* step = i-1 */
            seq_t prev = cache[i - 2];            /* a(i-1) */

            bool usable = false;
            if (prev >= step) {                   /* candidate is nonnegative */
                seq_t cand = prev - step;
                /* count occurrences of cand in cache[0 .. i-3] */
                size_t cnt = 0;
                for (idx_t k = 0; k < i - 2; ++k) {
                    if (cache[k] == cand) {
                        ++cnt;
                        if (cnt > 1) break;         /* no need to count further */
                    }
                }
                usable = (cnt <= 1);
            }

            cache[i - 1] = (usable ? prev - step : prev + step);
        }
        size = n;
    }

    return cache[n - 1];
}
