/*
 * A076601  a(1)=3, a(n_even)=(a(n-1)^2-1)/2; a(n_odd)=a(n-1)+1.
 * https://oeis.org/A076601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 3; a(n_even) = (a(n-1)^2 - 1) / 2; a(n_odd) = a(n-1) + 1
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 11, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* a(n) uses 1‑based indexing */
seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;   /* array of terms, cache[0] == a(1) */
    static size_t len = 0;        /* number of terms stored */
    static size_t cap = 0;        /* allocated capacity */

    /* grow cache if needed */
    if (len < n) {
        size_t new_cap = (cap ? cap * 2 : 8);
        if (new_cap < n) new_cap = n;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);        /* allocation failure */

        for (size_t i = 0; i < len; ++i) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cap   = new_cap;

        /* compute missing terms */
        for (size_t i = len; i < n; ++i) { /* i is 0‑based index corresponding to a(i+1) */
            idx_t idx = (idx_t)(i + 1); /* 1‑based index */
            if (idx == 1) {
                cache[i] = (seq_t)3;       /* a(1) = 3 */
            } else {
                seq_t prev = cache[i - 1]; /* a(idx-1) */
                if ((idx & 1) == 0) {      /* even idx */
                    seq_t sq  = prev * prev;
                    seq_t res = (sq - 1) / 2;
                    cache[i] = res;
                } else {                    /* odd idx */
                    cache[i] = prev + (seq_t)1;
                }
            }
        }
        len = n;
    }
    return cache[n - 1];
}
