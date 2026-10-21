/*
 * A080795  Number of minimax trees on n nodes.
 * https://oeis.org/A080795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 , a(1) = 1 a(n+1) = 2 * Σ_{k=0..n} binomial(n,k) * a(k)
 *            * a(n-k) – 0^n (Note: 0^0 is taken as 1, so the subtraction
 *            affects only a(1).)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>      /* malloc, free */

static seq_t *cache   = NULL;   /*  a[0] .. a[cache_len-1]  */
static idx_t  cache_len = 0;    /*  number of computed terms    */
static seq_t *binom    = NULL;   /*  binomial row for n = cache_len-1  */
static idx_t  binom_len = 0;    /*  length of current binom row   */

static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;              /* already computed */

    /* initialise on first use */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;                 /* a(0) = 1 */

        binom_len = 1;
        binom = (seq_t*)malloc(sizeof(seq_t));
        binom[0] = (seq_t)1;                 /* binom(0,0) = 1 */
    }

    while (cache_len <= n) {                /* compute a(cache_len) */
        idx_t new_n = cache_len;             /* new term index = cur */
        idx_t row    = new_n - 1;           /* binomial row needed for it */

        /* compute Σ_{k=0..row} C(row,k) * a(k) * a(row-k) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= row; ++k) {
            sum += binom[k] * cache[k] * cache[row - k];
        }
        sum *= (seq_t)2;
        if (row == 0) sum -= (seq_t)1;       /* subtract 0^0 for a(1) */

        /* grow cache to contain new_n */
        seq_t *tmp = (seq_t*)realloc(cache, (new_n + 1) * sizeof(seq_t));
        if (!tmp) { /* malloc failure – terminate */
            free(cache);
            free(binom);
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cache[new_n] = sum;
        ++cache_len;

        /* build next binomial row (length new_n+1 = row+2) */
        seq_t *new_binom = (seq_t*)malloc((new_n + 1) * sizeof(seq_t));
        if (!new_binom) {
            free(cache);
            free(binom);
            exit(EXIT_FAILURE);
        }
        new_binom[0] = (seq_t)1;
        for (idx_t i = 1; i < new_n; ++i) {
            new_binom[i] = binom[i - 1] + binom[i];
        }
        new_binom[new_n] = (seq_t)1;

        free(binom);
        binom = new_binom;
        binom_len = new_n + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
