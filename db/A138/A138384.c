/*
 * A138384  Reverse groups of five Fibonacci numbers.
 * https://oeis.org/A138384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-5) + a(n-10) a(0)=3, a(1)=2, a(2)=1, a(3)=1,
 *            a(4)=0, a(5)=34, a(6)=21, a(7)=13, a(8)=8, a(9)=5
 * Range:     u8 n <= 9, u16 n <= 24, u32 n <= 44, u64 n <= 89, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;

static void init_cache(void) {
    cache_cap = 10;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    cache[0] = (seq_t)3;
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)1;
    cache[3] = (seq_t)1;
    cache[4] = (seq_t)0;
    cache[5] = (seq_t)34;
    cache[6] = (seq_t)21;
    cache[7] = (seq_t)13;
    cache[8] = (seq_t)8;
    cache[9] = (seq_t)5;
}

static void compute_up_to(idx_t n) {
    if (!cache) init_cache();
    if (n < cache_cap) return;

    idx_t old_cap = cache_cap;
    cache_cap = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    cache = tmp;

    for (idx_t i = old_cap; i <= n; ++i) {
        cache[i] = (seq_t)11 * cache[i-5] + cache[i-10];
    }
}

seq_t a(idx_t n) {
    compute_up_to(n);
    return cache[n];
}
