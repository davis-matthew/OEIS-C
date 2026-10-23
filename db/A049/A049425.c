/*
 * A049425  Row sums of triangle A049404.
 * https://oeis.org/A049425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+3) = a(n+2) + 2*(n+2)*a(n+1) + (n+2)*(n+1)*a(n) (a(0)=1,
 *            a(1)=1, a(2)=3)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

static void ensure_cache_size(idx_t n)
{
    if (n < cache_len) return;
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* memory exhausted */
        a_cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    ensure_cache_size(n);
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                a_cache[0] = 1;
            } else if (i == 1) {
                a_cache[1] = 1;
            } else if (i == 2) {
                a_cache[2] = 3;
            } else {
                idx_t k = i - 3;
                seq_t a_k   = a_cache[k];
                seq_t a_k1  = a_cache[k + 1];
                seq_t a_k2  = a_cache[k + 2];
                seq_t term1 = a_k2;
                seq_t coeff2 = (seq_t)2 * (seq_t)(k + 2);
                seq_t prod1 = a_k1 * coeff2;
                seq_t coeff3 = (seq_t)(k + 2) * (seq_t)(k + 1);
                seq_t prod2 = a_k * coeff3;
                a_cache[i] = term1 + prod1 + prod2;
            }
        }
        cache_len = n + 1;
    }
    return a_cache[n];
}
