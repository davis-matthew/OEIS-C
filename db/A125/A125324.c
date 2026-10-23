/*
 * A125324  Number of base 14 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A125324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(M^n) for n≥1, a(0)=1 where M is a 14×14 matrix with
 *            M[i][j] = 1 if |i−j|≤3, else 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"

static seq_t base[14][14];
static seq_t power[14][14];
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static size_t current_n = 0;
static int initialized = 0;

static void grow_cache(size_t new_len) {
    if (new_len <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < new_len) new_cap *= 2;
    seq_t *new_cache = alloc_seq_array(new_cap);
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
    if (cache) seq_free_array(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

static void init(void) {
    /* Build adjacency matrix */
    for (size_t i = 0; i < 14; ++i) {
        for (size_t j = 0; j < 14; ++j) {
            base[i][j] = ( (idx_t)i > (idx_t)j ?
                ((idx_t)i - (idx_t)j) : ((idx_t)j - (idx_t)i) ) <= 3 ? (seq_t)1 : (seq_t)0;
        }
    }
    /* power = I */
    for (size_t i = 0; i < 14; ++i)
        for (size_t j = 0; j < 14; ++j)
            power[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
    /* Init cache with a(0) = 1 */
    grow_cache(1);
    cache[0] = (seq_t)1;
    cache_len = 1;
    current_n = 0;
    initialized = 1;
}

seq_t a(idx_t n) {
    if (!initialized) init();
    if (n == 0) return cache[0];
    /* Extend cache up to n */
    while (current_n < n) {
        /* power = power * base */
        seq_t temp[14][14];
        for (size_t i = 0; i < 14; ++i) {
            for (size_t j = 0; j < 14; ++j) {
                seq_t sum = 0;
                for (size_t k = 0; k < 14; ++k)
                    sum += power[i][k] * base[k][j];
                temp[i][j] = sum;
            }
        }
        for (size_t i = 0; i < 14; ++i)
            for (size_t j = 0; j < 14; ++j)
                power[i][j] = temp[i][j];
        /* trace */
        seq_t trace = 0;
        for (size_t i = 0; i < 14; ++i)
            trace += power[i][i];
        /* store */
        grow_cache(cache_len + 1);
        cache[cache_len] = trace;
        cache_len++;
        current_n++;
    }
    return cache[n];
}
