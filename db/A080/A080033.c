/*
 * A080033  a(n) is taken to be the smallest positive integer not already
 *          present which is consistent with the condition "n is a member of
 *          the sequence if and only if a(n) is a multiple of 4".
 * https://oeis.org/A080033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-8) - a(n-16) for n > 15, a(0)=0, a(1)=2, a(2)=4,
 *            a(3)=5, a(4)=8, a(5)=12, a(6)=7, a(7)=16, a(8)=20, a(9)=10,
 *            a(10)=24, a(11)=13, a(12)=28, a(13)=32, a(14)=15, a(15)=36
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

static const seq_t init_vals[16] = {
    (seq_t)0 , (seq_t)2 , (seq_t)4 , (seq_t)5 ,
    (seq_t)8 , (seq_t)12, (seq_t)7 , (seq_t)16,
    (seq_t)20, (seq_t)10, (seq_t)24, (seq_t)13,
    (seq_t)28, (seq_t)32, (seq_t)15, (seq_t)36
};

static seq_t *cache = NULL;      /* cached sequence values   */
static idx_t  cache_size = 0;    /* number of elements cached */

/* Grow the cache so that it contains at least indices 0..n. */
static void ensure_cache(idx_t n) {
    if (n < cache_size) return;           /* already have it */

    idx_t new_size = n + 1;
    /* realloc can be used with a NULL pointer; cast is required for C++ */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();              /* out of memory */
    cache = new_cache;

    /* If this is the first allocation, initialize the first 16 terms. */
    if (cache_size == 0) {
        idx_t init_len = (new_size < 16) ? new_size : 16;
        for (idx_t i = 0; i < init_len; ++i)
            cache[i] = init_vals[i];
        cache_size = init_len;
    }

    /* Compute any new entries beyond what we already have. */
    for (idx_t i = cache_size; i < new_size; ++i) {
        if (i < 16) {                         /* falls back to init_vals */
            cache[i] = init_vals[i];
        } else {
            cache[i] = (seq_t)((seq_t)2 * cache[i - 8] - cache[i - 16]);
        }
    }
    cache_size = new_size;
}

/* External entry point required by the build */
seq_t a(idx_t n) {
    assert(n >= 0);
    ensure_cache(n);
    return cache[n];
}
