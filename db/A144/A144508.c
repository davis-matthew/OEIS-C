/*
 * A144508  a(n) = total number of partitions of [1, 2, ..., k] into exactly n
 *          blocks, each of size 1, 2, 3 or 4, for 0 <= k <= 4n.
 * https://oeis.org/A144508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^{4n} p(n,k) where p(0,0)=1, p(0,k>0)=0 and for
 *            n>0, p(n,k) = sum_{s=1}^{4} C(k-1,s-1) * p(n-1,k-s)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* the function below computes a(n) for any n>=0 */

#include <stdlib.h>   /* malloc, calloc, free, realloc */

static inline seq_t small_binom(idx_t a, idx_t r)
{
    if (r == 0) return (seq_t)1;
    if (r == 1) return (seq_t)a;
    if (r == 2) return ((seq_t)a * (seq_t)(a - 1)) / (seq_t)2;
    if (r == 3) return ((seq_t)a * (seq_t)(a - 1) * (seq_t)(a - 2)) / (seq_t)6;
    return (seq_t)0;  /* unreachable for r<=3 */
}

seq_t a(idx_t n)
{
    /* static state: cache of a(n) values and current DP arrays */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed_up_to = 0;   /* highest n currently cached */

    static seq_t *prev_buf = NULL;  /* P[n-1][*] */
    static size_t prev_size = 0;    /* length of prev_buf */

    /* initialization on first call */
    if (cache == NULL) {
        cache = (seq_t*)calloc(1, sizeof(seq_t));
        cache_cap = 1;
        cache[0] = (seq_t)1;       /* a(0) = 1 */

        prev_buf = (seq_t*)calloc(1, sizeof(seq_t));
        prev_size = 1;
        prev_buf[0] = (seq_t)1;    /* p(0,0) = 1 */

        computed_up_to = 0;
    }

    /* extend computations up to requested n */
    if (n > computed_up_to) {
        for (idx_t cur_n = computed_up_to + 1; cur_n <= n; ++cur_n) {
            /* allocate current DP array for p(cur_n, k) */
            size_t cur_size = 4 * cur_n + 1;
            seq_t *cur_buf = (seq_t*)calloc(cur_size, sizeof(seq_t));

            /* compute p(cur_n, k) for all k */
            for (size_t k = 0; k < cur_size; ++k) {
                seq_t sum = (seq_t)0;
                for (idx_t s = 1; s <= 4; ++s) {
                    if (k >= s) {
                        idx_t rk = k - s;
                        if (rk < prev_size) {
                            seq_t comb = small_binom(k - 1, s - 1);
                            sum += comb * prev_buf[rk];
                        }
                    }
                }
                cur_buf[k] = sum;
            }

            /* compute a(cur_n) = sum_{k} p(cur_n, k) */
            seq_t a_cur = (seq_t)0;
            for (size_t i = 0; i < cur_size; ++i)
                a_cur += cur_buf[i];

            /* store a_cur in the cache */
            if (cur_n >= cache_cap) {
                size_t new_cap = cur_n + 1;
                seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) exit(1);   /* allocation failure */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cur_n] = a_cur;

            /* prepare for next iteration */
            free(prev_buf);
            prev_buf = cur_buf;
            prev_size = cur_size;
            computed_up_to = cur_n;
        }
    }

    /* return cached value */
    return cache[n];
}
