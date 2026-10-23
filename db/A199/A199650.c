/*
 * A199650  Number of nX3 0..2 arrays with values 0..2 introduced in row major
 *          order and each element equal to no more than two horizontal and
 *          vertical neighbors.
 * https://oeis.org/A199650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence relation for n > 10 a(n) = 21*a(n-1) + 44*a(n-2) +
 *            8*a(n-3) - 96*a(n-4) - 364*a(n-5) - 264*a(n-6) + 256*a(n-7) +
 *            288*a(n-8) + 64*a(n-9) Initial values: a(1)=5, a(2)=113,
 *            a(3)=2584, a(4)=59212, a(5)=1357484, a(6)=31120632,
 *            a(7)=713445976, a(8)=16355879648, a(9)=374961537040,
 *            a(10)=85960...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static const seq_t init_vals[10] = {
    (seq_t)5ULL,
    (seq_t)113ULL,
    (seq_t)2584ULL,
    (seq_t)59212ULL,
    (seq_t)1357484ULL,
    (seq_t)31120632ULL,
    (seq_t)713445976ULL,
    (seq_t)16355879648ULL,
    (seq_t)374961537040ULL,
    (seq_t)8596061907680ULL
};

static void ensure_cache(idx_t n) {
    if (n <= cache_size) return;
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    cache = new_cache;
    for (idx_t i = cache_size; i < n; ++i) {
        if (i < 10) {
            cache[i] = init_vals[i];
        } else {
            seq_t sum = cache[i-1] * (seq_t)21
                      + cache[i-2] * (seq_t)44
                      + cache[i-3] * (seq_t)8
                      - cache[i-4] * (seq_t)96
                      - cache[i-5] * (seq_t)364
                      - cache[i-6] * (seq_t)264
                      + cache[i-7] * (seq_t)256
                      + cache[i-8] * (seq_t)288
                      + cache[i-9] * (seq_t)64;
            cache[i] = sum;
        }
    }
    cache_size = n;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    ensure_cache(n);
    return cache[n-1];
}
