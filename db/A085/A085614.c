/*
 * A085614  Number of elementary arches of size n.
 * https://oeis.org/A085614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A085614: Number of elementary arches of size n
 *  Recurrence (OEIS notation):
 *      a(1) = 1, a(2) = 3, a(3) = 16
 *      for n >= 4:
 *          a(n) = (- (n-1)*(n-2)*a(n-1)
 *                  + 12*(3*n-5)*(3*n-7)*a(n-2)
 *                  + 12*(3*n-8)*(3*n-10)*a(n-3))
 *                 / (n*(n-1))
 *
 *  The recurrence produces integers; intermediate values may need 128‑bit
 *  arithmetic for 64‑bit seq_t.  The implementation caches results
 *  so that calls are O(1) after the first computation for a given n.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;        /* number of terms cached */

    if (n == 0ULL) /* offset is 1, so a(0) is undefined; return 0 */
        return (seq_t)0;

    /* grow cache if necessary */
    if ((size_t)n > cache_len) {
        cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* allocation failed */

        /* compute terms that are not yet cached */
        for (idx_t i = (idx_t)cache_len; i < n; ++i) {
            idx_t idx = i + 1;   /* actual sequence index (1‑based) */

            if (idx == 1)          /* a(1) */
                cache[i] = (seq_t)1;
            else if (idx == 2)     /* a(2) */
                cache[i] = (seq_t)3;
            else if (idx == 3)     /* a(3) */
                cache[i] = (seq_t)16;
            else { /* recurrence for idx >= 4 */
                seq_t an_1 = cache[i-1];   /* a(n-1) */
                seq_t an_2 = cache[i-2];   /* a(n-2) */
                seq_t an_3 = cache[i-3];   /* a(n-3) */

                if (sizeof(seq_t) <= 8) { /* 64‑bit or smaller: use 128‑bit intrinsics */
                    unsigned __int128 term1 = (unsigned __int128)(idx-1) *
                                               (unsigned __int128)(idx-2) *
                                               (unsigned __int128)an_1;

                    unsigned __int128 term2 = (unsigned __int128)12 *
                                               (unsigned __int128)(3*idx-5) *
                                               (unsigned __int128)(3*idx-7) *
                                               (unsigned __int128)an_2;

                    unsigned __int128 term3 = (unsigned __int128)12 *
                                               (unsigned __int128)(3*idx-8) *
                                               (unsigned __int128)(3*idx-10) *
                                               (unsigned __int128)an_3;

                    unsigned __int128 numerator = term2 + term3 - term1;
                    unsigned __int128 denominator = (unsigned __int128)idx *
                                                    (unsigned __int128)(idx-1);

                    cache[i] = (seq_t)(numerator / denominator);
                } else { /* arbitrary‑precision seq_t: use native arithmetic */
                    seq_t term1 = (seq_t)(idx-1) * (idx-2) * an_1;
                    seq_t term2 = (seq_t)12 * (seq_t)(3*idx-5) *
                                  (seq_t)(3*idx-7) * an_2;
                    seq_t term3 = (seq_t)12 * (seq_t)(3*idx-8) *
                                  (seq_t)(3*idx-10) * an_3;

                    seq_t numerator = term2 + term3 - term1;
                    seq_t denominator = (seq_t)idx * (idx-1);

                    cache[i] = numerator / denominator;
                }
            }
        }
        cache_len = (size_t)n;
    }

    return cache[n-1];
}
