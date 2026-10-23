/*
 * A027406  Number of labeled servers of dimension 19.
 * https://oeis.org/A027406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A027406: Number of labeled servers of dimension 19
   a(n) = number of labeled structures with exponential generating function
          A(x) = exp( sum_{i=1}^{19} ((1+x)^i - 1)/i ).
   Recurrence derived from the derivative identity A' = F' A:
   a(0) = 1
   for n >= 1
       a(n) = Σ_{k=1}^{min(n,19)} c_k * a(n-k) * (n-1)! / (n-k)!,
       where c_k = k * f_k and f_k = Σ_{i=k}^{19} binom(i,k)/i.
   The constants c_k are integers and can be pre‑computed once.
*/

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Pre‑compute the constants c_k = k * Σ_{i=k}^{19} binom(i,k)/i.
       For k > 19, c_k = 0. */
    static seq_t c[20] = {0};
    static int c_init = 0;
    if (!c_init) {
        for (int k = 1; k <= 19; ++k) {
            seq_t sum = 0;
            for (int i = k; i <= 19; ++i) {
                /* binom(i-1, k-1) = (i-1)! / ((k-1)! (i-k)!) */
                int kk = k - 1;
                int nn = i - 1;
                int rr = kk < nn - kk ? kk : nn - kk;
                seq_t r = 1;
                for (int j = 1; j <= rr; ++j) {
                    r = r * (nn - rr + j);
                    r = r / j;
                }
                sum += r;
            }
            c[k] = sum;              /* c_k = k * f_k */
        }
        c_init = 1;
    }

    /* Simple cache for previously computed values. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* Ensure cache can hold up to n. */
    if (n >= cache_len) {
        size_t need = n + 1;
        if (need > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < need) new_cap *= 2;
            seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
            if (!new_cache) return 0;        /* abort on OOM */
            if (cache) {
                memcpy(new_cache, cache, cache_len * sizeof(seq_t));
                free(cache);
            }
            memset(new_cache + cache_len, 0, (new_cap - cache_len) * sizeof(seq_t));
            cache = new_cache;
            cache_cap = new_cap;
        }
        /* Compute new entries. */
        for (size_t m = cache_len; m <= (size_t)n; ++m) {
            if (m == 0) {
                cache[0] = (seq_t)1;
                continue;
            }
            seq_t sum = 0;
            seq_t prod = 1;                     /* prod = (m-1)!/(m-i)! for current i */
            size_t imax = m < 19 ? m : 19;
            for (size_t i = 1; i <= imax; ++i) {
                if (i > 1) prod = prod * (seq_t)(m - i + 1);
                seq_t term = cache[m - i] * c[i];
                term = term * prod;
                sum = sum + term;
            }
            cache[m] = sum;
        }
        cache_len = n + 1;
    }
    return cache[(size_t)n];
}
