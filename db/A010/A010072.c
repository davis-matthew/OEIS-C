/*
 * A010072  a(n+1) = a(n) + sum of digits in base 9 representation of a(n).
 * https://oeis.org/A010072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n+1) = a(n) + sum of the digits of a(n) in base 9
 * Range:     u8 n <= 29, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A010072 */

#include <stdlib.h>

/* Computes the n-th term of the sequence where a(0)=1 and
   a(n+1) = a(n) + sum of the base‑9 digits of a(n).  The
   function is memoized; it can handle calls in any order. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;     /* array of computed terms   */
    static idx_t cache_size = 0;    /* number of terms currently valid */
    static size_t alloc_size = 0;   /* size of the allocated array     */

    /* If we already know a(n), return it immediately */
    if (n < cache_size)
        return cache[n];

    /* Target size of the cache (must be at least n+1) */
    size_t target_size = (size_t)n + 1;

    /* Allocate or grow the cache as needed */
    if (cache == NULL) {
        cache = (seq_t *)malloc(target_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory – unlikely in tests */
        alloc_size = target_size;
        cache_size  = 0;
    } else if (target_size > alloc_size) {
        seq_t *tmp = (seq_t *)realloc(cache, target_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* realloc failed */
        cache   = tmp;
        alloc_size = target_size;
    }

    /* Initialise the first term if we have just allocated the cache */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* Fill the cache up to the requested index */
    const seq_t nine = (seq_t)9;
    for (size_t i = (size_t)cache_size; i < target_size; ++i) {
        seq_t prev = cache[i - 1];
        seq_t sum  = (seq_t)0;
        seq_t tmp  = prev;
        while (tmp > (seq_t)0) {
            sum += tmp % nine;
            tmp /= nine;
        }
        cache[i] = prev + sum;
    }

    cache_size = (idx_t)target_size;
    return cache[n];
}
