/*
 * A121496  Run lengths of successive numbers in A068225.
 * https://oeis.org/A121496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-4)-a(n-5) (n > 5) Initial values: a(1)=1,
 *            a(2)=2, a(3)=2, a(4)=1, a(5)=3
 * Range:     u8 n <= 509, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* 0‑based array of computed values */
    static size_t cache_cnt = 0;     /* current number of stored values */

    if (n == 0) return 0;           /* undefined for n < 1 */

    /* Ensure we have values up to n */
    if ((size_t)n > cache_cnt) {
        size_t new_cnt = (size_t)n;
        seq_t *new_cache = (seq_t *)malloc(new_cnt * sizeof(seq_t));
        if (cache) {
            for (size_t i = 0; i < cache_cnt; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;

        /* Compute new values */
        for (size_t i = cache_cnt; i < new_cnt; ++i) {
            idx_t idx = (idx_t)(i + 1);   /* 1‑based index of the term */
            seq_t val;
            if (idx == 1)           val = 1;
            else if (idx == 2)      val = 2;
            else if (idx == 3)      val = 2;
            else if (idx == 4)      val = 1;
            else if (idx == 5)      val = 3;
            else {
                seq_t a1 = cache[idx - 2];    /* a(n-1) */
                seq_t a4 = cache[idx - 5];    /* a(n-4) */
                seq_t a5 = cache[idx - 6];    /* a(n-5) */
                val = a1 + a4 - a5;
            }
            cache[i] = val;
        }
        cache_cnt = new_cnt;
    }

    return cache[n - 1];
}
