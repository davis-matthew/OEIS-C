/*
 * A140061  Triangle of quotients.
 * https://oeis.org/A140061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   Q(n,1)=n; for k>1, Q(n,k)=k*⌊Q(n,k-1)/k⌋. The sequence a(n)
 *            consists of the rows of Q(n,k) in which all entries are
 *            distinct, written row‑major
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* candidate start values that are tested for rows */
    static uint64_t cand = 1;

    /* initialise cache */
    if (!cache) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate rows until we have at least n terms in the cache */
    while (cache_len < (size_t)n) {
        seq_t prev = 0;
        seq_t val = (seq_t)cand;

        bool distinct = true;
        seq_t row[128];
        size_t row_len = 0;

        for (size_t k = 1; ; ++k) {
            if (k == 1) {
                val = (seq_t)cand;
            } else {
                val = ((seq_t)k) * (prev / (seq_t)k);
            }
            if (val == (seq_t)0) break;

            /* check for duplicates within this row */
            for (size_t j = 0; j < row_len; ++j) {
                if (val == row[j]) {
                    distinct = false;
                    break;
                }
            }
            if (!distinct) break;

            if (row_len < sizeof(row) / sizeof(row[0]))
                row[row_len++] = val;
            else
                distinct = false; /* too long – treat as non‑distinct */
            prev = val;
        }

        /* if the candidate yields a distinct row, store its entries */
        if (distinct && row_len > 0) {
            if (cache_len + row_len > cache_cap) {
                size_t new_cap = cache_cap * 2;
                if (new_cap < cache_len + row_len) new_cap = cache_len + row_len;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            for (size_t i = 0; i < row_len; ++i)
                cache[cache_len++] = row[i];
        }

        cand++; /* next candidate start value */
    }

    return cache[n - 1];
}
