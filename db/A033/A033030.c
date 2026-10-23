/*
 * A033030  Derangement numbers d(n,3) where d(n,k) = k(n-1)(d(n-1,k) +
 *          d(n-2,k)), with d(0,k) = 1 and d(1,k) = 0.
 * https://oeis.org/A033030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(n-1)*(a(n-1)+a(n-2)), a(0)=1, a(1)=0;
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static bool *ready = NULL;
static idx_t stored = 0;

/* Ensure array can hold indices up to n. */
static void ensure_capacity(idx_t n)
{
    if (n < stored)
        return;
    idx_t new_size = stored ? stored << 1 : 16;
    while (new_size <= n)
        new_size <<= 1;
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    bool *new_ready = (bool *)malloc(new_size * sizeof(bool));
    for (idx_t i = 0; i < new_size; ++i)
        new_ready[i] = false;
    for (idx_t i = 0; i < stored; ++i) {
        new_cache[i] = cache[i];
        new_ready[i] = ready[i];
    }
    free(cache);
    free(ready);
    cache = new_cache;
    ready = new_ready;
    stored = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;

    ensure_capacity(n);

    if (!ready[0]) {
        cache[0] = (seq_t)1;
        ready[0] = true;
    }
    if (!ready[1]) {
        cache[1] = (seq_t)0;
        ready[1] = true;
    }

    for (idx_t i = 2; i <= n; ++i) {
        if (!ready[i]) {
            seq_t sum = cache[i - 1] + cache[i - 2];
            seq_t factor = (seq_t)3 * (seq_t)(i - 1);
            cache[i] = factor * sum;
            ready[i] = true;
        }
    }
    return cache[n];
}
