/*
 * A024796  Numbers expressible in more than one way as i^2 + j^2 + k^2, where
 *          1 <= i <= j <= k.
 * https://oeis.org/A024796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 8057, u64 n <= 8057, u128 n <= 8057,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the nth integer that can be expressed in more than one
 * way as i^2 + j^2 + k^2 with 1 <= i <= j <= k.
 * The sequence starts: 27, 33, 38, 41, 51, 54, 57, 59, 62, 66, ...
 */

#include <stdlib.h>
#include <stddef.h>

/* comparison function for qsort */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/*
 * Compute a(n). The function caches the whole list of qualifying sums
 * after the first call; subsequent calls simply index into the cache.
 */
seq_t a(idx_t n)
{
    /* static cache variables */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* if we have not yet built the cache, do it */
    if (cache_len == 0) {
        /* upper bound for k; 500 gives >10 000 terms (more than enough) */
        const idx_t MAX_K = 500;
        /* maximum possible sum with i=j=k=MAX_K */
        const idx_t maxSum = 3 * MAX_K * MAX_K;   /* 750 000 */

        /* count array: we only need 0/1/2 counts, so unsigned char */
        unsigned char *cnt = (unsigned char *)calloc((size_t)maxSum + 1, 1);
        if (!cnt) return 0;   /* allocation failure → give up */

        /* pre‑compute squares */
        seq_t *sq = (seq_t *)malloc((size_t)(MAX_K + 1) * sizeof(seq_t));
        if (!sq) { free(cnt); return 0; }
        for (idx_t i = 0; i <= MAX_K; ++i) sq[i] = (seq_t)i * (seq_t)i;

        /* dynamic array for the sums that occur at least twice */
        size_t vals_cap = 200000;                 /* generous initial cap */
        seq_t *vals = (seq_t *)malloc(vals_cap * sizeof(seq_t));
        if (!vals) { free(cnt); free(sq); return 0; }
        size_t vals_len = 0;

        /* generate all i<=j<=k triples and count sums */
        for (idx_t i = 1; i <= MAX_K; ++i) {
            seq_t sq_i = sq[i];
            for (idx_t j = i; j <= MAX_K; ++j) {
                seq_t sum_ij = sq_i + sq[j];
                for (idx_t k = j; k <= MAX_K; ++k) {
                    seq_t sum = sum_ij + sq[k];
                    size_t idx = (size_t)sum;   /* sum fits in native size */
                    if (cnt[idx] < 2) {
                        ++cnt[idx];
                        if (cnt[idx] == 2) {   /* first time it appears twice */
                            if (vals_len == vals_cap) {
                                vals_cap *= 2;
                                seq_t *tmp = (seq_t *)realloc(vals,
                                     vals_cap * sizeof(seq_t));
                                if (!tmp) { free(cnt); free(sq); free(vals);
                                            return 0; }
                                vals = tmp;
                            }
                            vals[vals_len++] = sum;
                        }
                    }
                }
            }
        }

        /* we no longer need the count array or squares */
        free(cnt);
        free(sq);

        /* sort the qualifying sums */
        qsort(vals, vals_len, sizeof(seq_t), cmp_seq);

        /* cache the result */
        cache = vals;
        cache_len = vals_len;
    }

    if (n <= 0)                return 0; /* undefined for n<1 */
    if ((size_t)n > cache_len) return 0; /* out of pre‑computed range */

    return cache[(size_t)n - 1];
}
