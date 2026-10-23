/*
 * A163962  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A163962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 15, a(2) = 210, a(3) = 2940, a(4) = 41160, a(5)
 *            = 576240, a(6) = 8067255 For n >= 7: a(n) =
 *            13*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)) - 91*a(n-6) (This
 *            recurrence follows from the generating function in the OEIS
 *            entry.)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* cached values of the sequence */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of values that are already in cache   */
static idx_t cache_cap = 0;   /* current allocated capacity (>= cache_len)   */

/* grow the cache so that it can hold values up to index n (inclusive)  */
static void grow_cache(idx_t n)
{
    if (n < cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* if realloc fails, abort – the program is not expected to continue */
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initial terms a(0) .. a(6) */
    static const seq_t base[7] = {
        (seq_t)1,            /* a(0) */
        (seq_t)15,           /* a(1) */
        (seq_t)210,          /* a(2) */
        (seq_t)2940,         /* a(3) */
        (seq_t)41160,        /* a(4) */
        (seq_t)576240,       /* a(5) */
        (seq_t)8067255       /* a(6) */
    };

    grow_cache(n);
    if (cache_len > n) return cache[n];

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i <= 6) {
            val = base[i];
        } else {
            /* a(i) = 13*(a(i-1)+...+a(i-5)) - 91*a(i-6) */
            seq_t s = cache[i-1];
            s += cache[i-2];
            s += cache[i-3];
            s += cache[i-4];
            s += cache[i-5];
            val = 13 * s - 91 * cache[i-6];
        }
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
