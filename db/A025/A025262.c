/*
 * A025262  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ... + a(n-1)*a(1) for n >= 4.
 * https://oeis.org/A025262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n-1} a(k)*a(n-k), with a(1)=a(2)=a(3)=1
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 22, u64 n <= 39, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t cache_cap = 0;

static void grow_cache(idx_t n_needed) {
    if (cache_cap >= n_needed + 1) return;
    idx_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
    while (new_cap <= n_needed) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    for (idx_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
    seq_cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;          // undefined for n<1
    grow_cache(n);
    if (n <= 3) return 1;         // base values

    // initialize first three if not yet
    if (seq_cache[1] == 0) seq_cache[1] = 1;
    if (seq_cache[2] == 0) seq_cache[2] = 1;
    if (seq_cache[3] == 0) seq_cache[3] = 1;

    // compute values up to n
    for (idx_t i = 4; i <= n; ++i) {
        if (seq_cache[i] != 0) continue; // already computed
        seq_t sum = 0;
        for (idx_t k = 1; k < i; ++k) {
            sum += seq_cache[k] * seq_cache[i - k];
        }
        seq_cache[i] = sum;
    }
    return seq_cache[n];
}
