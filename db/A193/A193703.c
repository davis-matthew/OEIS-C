/*
 * A193703  Number of arrays of -1..1 integers x(1..n) with every x(i) in a
 *          subsequence of length 1, 2, 3 or 4 with sum zero.
 * https://oeis.org/A193703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 2*a(n-3) - a(n-4) + 3*a(n-6) +
 *            4*a(n-7); Initial values: a(1)=1, a(2)=3, a(3)=9, a(4)=27,
 *            a(5)=73, a(6)=195, a(7)=515
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifdef __cplusplus
#include <new>
#endif

/* Internal memoization storage */
static seq_t *cache = NULL;
static idx_t max_n = 0;
static size_t capacity = 0;

/* Ensure the cache can hold at least n elements (index 0..n). */
static void ensure_capacity(idx_t n)
{
    if (n < capacity)
        return;
    size_t new_cap = capacity ? capacity << 1 : 8;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_cap];
    if (cache) {
        for (size_t i = 0; i < capacity; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
    }
    cache = new_cache;
#else
    seq_t *new_cache = (seq_t*)calloc(new_cap, sizeof(seq_t));
    if (cache) {
        memcpy(new_cache, cache, capacity * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
#endif
    capacity = new_cap;
}

/* Sequence term a(n),  n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n = 0, but harmless */

    ensure_capacity(n);

    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i == 1)
            cache[1] = (seq_t)1;
        else if (i == 2)
            cache[2] = (seq_t)3;
        else if (i == 3)
            cache[3] = (seq_t)9;
        else if (i == 4)
            cache[4] = (seq_t)27;
        else if (i == 5)
            cache[5] = (seq_t)73;
        else if (i == 6)
            cache[6] = (seq_t)195;
        else if (i == 7)
            cache[7] = (seq_t)515;
        else {
            seq_t t = 2 * cache[i-1];
            t += cache[i-2];
            t += 2 * cache[i-3];
            t -= cache[i-4];
            t += 3 * cache[i-6];
            t += 4 * cache[i-7];
            cache[i] = t;
        }
    }
    max_n = n;
    return cache[n];
}
