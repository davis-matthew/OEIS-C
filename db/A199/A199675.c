/*
 * A199675  Expansion of e.g.f. 1/(exp(-x) - Sum_{n>=0}
 *          (-x)^(3*n+2)/(3*n+2)!).
 * https://oeis.org/A199675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; For n>0: a(n) = Σ_{1≤k≤n, k%3≠2}
 *            binom(n,k)·(-1)^{k+1}·a(n−k)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This recurrence follows from exp‑generating‑function inversion: */
#include <stdlib.h>   /* for malloc, realloc */
#include <stdint.h>  /* for uintptr_t if needed */

static seq_t *cache = NULL;     /* memoised values a(0)…a(N)    */
static idx_t  cache_len = 0;    /* number of entries in cache   */

seq_t a(idx_t n) {
    /* allocate (and extend) cache if necessary */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!new_cache) {
            /* allocation failure – abort (runtime error) */
            exit(EXIT_FAILURE);
        }
        cache = new_cache;
        /* initialise new entries */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;   /* a(0) */
            cache_len = 1;
        }
        for (idx_t cur = cache_len; cur <= n; ++cur) {
            if (cur == 0) {
                cache[0] = (seq_t)1;
                continue;
            }
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;          /* binom(cur,0) */
            for (idx_t k = 1; k <= cur; ++k) {
                /* update binomial coefficient: C(cur,k) = C(cur,k-1)*(cur-k+1)/k */
                binom = binom * (seq_t)(cur - k + 1) / (seq_t)k;
                if ((k % 3) == 2) continue;            /* d(k)=0 for k≡2 (mod 3) */
                seq_t term = binom * cache[cur - k];
                if ((k & 1) == 0) term = -term;        /* sign = (-1)^{k+1} */
                sum += term;
            }
            cache[cur] = sum;                       /* a(cur) */
        }
        cache_len = new_len;
    }
    return cache[n];
}
