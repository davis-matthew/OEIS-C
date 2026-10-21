/*
 * A230548  Twin hearts patterns packing into n X n coins.
 * https://oeis.org/A230548
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n‑2) + a(n‑3) - a(n‑5) + a(n‑6) - a(n‑8) - a(n‑9) +
 *            a(n‑11) Initial values: a(0)=0, a(1)=1, a(2)=0, a(3)=1, a(4)=2,
 *            a(5)=3, a(6)=6, a(7)=7, a(8)=8, a(9)=12, a(10)=15, a(11)=16 The
 *            recurrence is obtained from the rational generating function
 *            given in the OEIS entry. It holds for all n ≥ 12
 * Range:     u8 n <= 39, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* array holding computed a(n) values      */
static idx_t  cache_size = 0;     /* number of entries stored (max index+1)  */

/* build the cache up to index n (inclusive) if necessary */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;     /* already available */

    idx_t new_size = n + 1;
    if (cache_size == 0) {
        seq_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    } else {
        seq_cache = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
    }

    /* initialise new entries */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i <= 1) {
            /* a(0)=0, a(1)=1 */
            seq_cache[i] = (seq_t)(i == 1 ? 1 : 0);
        } else if (i <= 11) {
            /* hard‑coded values for 2 … 11 */
            static const seq_t init_vals[12] =
                {0, 1, 0, 1, 2, 3, 6, 7, 8, 12, 15, 16};
            seq_cache[i] = init_vals[i];
        } else {
            /* recurrence a(n) = a(n-2)+a(n-3)+a(n-6)+a(n-11)
             *                - a(n-5)-a(n-8)-a(n-9)                      */
            seq_t val = seq_cache[i - 2] + seq_cache[i - 3];
            val += seq_cache[i - 6];
            val += seq_cache[i - 11];
            val -= seq_cache[i - 5];
            val -= seq_cache[i - 8];
            val -= seq_cache[i - 9];
            seq_cache[i] = val;
        }
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                  /* undefined below offset   */
    ensure_cache(n);
    return seq_cache[n];
}
