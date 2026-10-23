/*
 * A163958  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A163958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -66*a(n-6) + 11*Sum_{k=1..5} a(n-k) for n >= 7 initial
 *            values: a(0)=1, a(1)=13, a(2)=156, a(3)=1872, a(4)=22464,
 *            a(5)=269568, a(6)=3234738
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

static void ensure_cache_size(idx_t needed)
{
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();  /* memory allocation failed */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < cache_len) return cache[n];
    if (cache_len == 0) {
        ensure_cache_size(7);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)13;
        cache[2] = (seq_t)156;
        cache[3] = (seq_t)1872;
        cache[4] = (seq_t)22464;
        cache[5] = (seq_t)269568;
        cache[6] = (seq_t)3234738;
        cache_len = 7;
    }
    if (n < cache_len) return cache[n];
    ensure_cache_size(n + 1);
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= 5; ++k) {
            sum += cache[i - k];
        }
        seq_t term = (seq_t)11 * sum;
        term -= (seq_t)66 * cache[i - 6];
        cache[i] = term;
    }
    cache_len = n + 1;
    return cache[n];
}
