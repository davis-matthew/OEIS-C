/*
 * A050042  a(n) = a(n-1) + a(m) for n >= 4, where m = n - 1 - 2^p and p is
 *          the unique integer such that 2^p < n - 1 <= 2^(p+1), starting with
 *          a(1) = 1, a(2) = 2, and a(3) = 1.
 * https://oeis.org/A050042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(m) for n ≥ 4 where m = n-1-2^p and p is the
 *            unique integer with 2^p < n-1 ≤ 2^(p+1) Initial values: a(1)=1,
 *            a(2)=2, a(3)=1. This implementation memoizes the sequence so
 *            calls may arrive in any order
 * Range:     u8 n <= 42, u16 n <= 208, u32 n <= 1804, u64 n <= 8193,
 *            u128 n <= 8193, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

#ifdef __cplusplus
#include <new>
#endif

/* static cache array (index 0 unused) */
static seq_t *cache = NULL;
static size_t cap   = 0;      /* allocated capacity */
static idx_t last   = 0;      /* largest n for which cache[n] is defined */

/* allocate an array of seq_t objects */
static seq_t *alloc_seq_array(size_t len) {
#ifdef __cplusplus
    return new seq_t[len];
#else
    return (seq_t*)malloc(len * sizeof(seq_t));
#endif
}

/* free an array of seq_t objects */
static void dealloc_seq_array(seq_t *ptr) {
#ifdef __cplusplus
    delete[] ptr;
#else
    free(ptr);
#endif
}

/* reallocate the cache to a new capacity, copying existing values */
static seq_t *resize_seq_array(seq_t *old, size_t old_cap, size_t new_cap) {
    seq_t *new_arr = alloc_seq_array(new_cap);
    size_t copy_len = old_cap < new_cap ? old_cap : new_cap;
    for (size_t i = 0; i < copy_len; ++i)
        new_arr[i] = old[i];
    dealloc_seq_array(old);
    return new_arr;
}

/* ensure the cache can hold entries up to n (inclusive) */
static void ensure_capacity(idx_t n) {
    if ((size_t)n + 1 <= cap)
        return;
    size_t new_cap = ((size_t)n + 1) * 2;
    if (new_cap == 0)
        new_cap = 4;
    cache = resize_seq_array(cache, cap, new_cap);
    cap   = new_cap;
}

/* initialize cache with the known base cases */
static void initialize_cache(void) {
    if (cap >= 4)
        return;
    cap = 8;                     /* spare space */
    cache = alloc_seq_array(cap);
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)1;
    last   = 3;
}

/* public sequence function */
seq_t a(idx_t n) {
    /* handle base cases directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)1;

    /* initialise and use the memoised cache */
    if (!cache)
        initialize_cache();

    if (n <= last)
        return cache[n];

    ensure_capacity(n);

    for (idx_t i = last + 1; i <= n; ++i) {
        idx_t exp   = floor_log2_uint64(i - 1);          /* p₀ = floor(log₂(i-1)) */
        idx_t pow2  = (1ULL << exp);
        if (pow2 == i - 1) {                            /* adjust when i-1 is a power of two */
            exp--;
            pow2 = (1ULL << exp);
        }
        idx_t m = i - 1 - pow2;                         /* m = i-1-2^p */
        cache[i] = cache[i - 1] + cache[m];
    }
    last = n;
    return cache[n];
}
