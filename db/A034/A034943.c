/*
 * A034943  Binomial transform of Padovan sequence A000931.
 * https://oeis.org/A034943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3); \ a(0) = a(1) = a(2) = 1
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 28, u64 n <= 54, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Reallocate an array of seq_t, preserving values and handling constructors
   in C++ and malloc/realloc in C. */
static void* realloc_seq_array(void* ptr, size_t old_size, size_t new_size)
{
#ifdef __cplusplus
    seq_t* old_arr = static_cast<seq_t*>(ptr);
    seq_t* new_arr = new seq_t[new_size];
    size_t copy = old_size < new_size ? old_size : new_size;
    for (size_t i = 0; i < copy; ++i)
        new_arr[i] = old_arr[i];
    delete[] old_arr;
    return static_cast<void*>(new_arr);
#else
    return realloc(ptr, new_size * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;

    /* Grow the cache if needed. */
    if ((size_t)n >= cache_capacity) {
        size_t new_capacity = (size_t)n + 1;
        cache = (seq_t*)realloc_seq_array(cache, cache_capacity, new_capacity);
        /* Compute missing terms starting at old capacity. */
        for (size_t i = cache_capacity; i < new_capacity; ++i) {
            if (i == 0 || i == 1 || i == 2) {
                cache[i] = 1;
            } else {
                cache[i] = 3 * cache[i - 1];
                cache[i] -= 2 * cache[i - 2];
                cache[i] += cache[i - 3];
            }
        }
        cache_capacity = new_capacity;
    }
    return cache[(size_t)n];
}
