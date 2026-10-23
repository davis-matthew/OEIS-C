/*
 * A161130  Sum of the differences between the largest and the smallest fixed
 *          points over all non-derangement permutations of {1,2,...,n}.
 * https://oeis.org/A161130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   (n-2)*a(n) = (n^2-2*n-1)*a(n-1) + (n-1)*n*a(n-2) a(0)=0, a(1)=0,
 *            a(2)=1 Implementation uses recursion with memoisation
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes required for the standard types and dynamic allocation. */
#include <stdlib.h>
#include <stdint.h>

/* Static cache structure to store computed values. */
struct _seq_cache_s {
    seq_t *data;          /* pointer to the cache array */
    idx_t size;           /* current allocated capacity */
    idx_t computed_to;    /* highest index that has been computed */
} _a_cache = {NULL, 0, 0};

/* Public function required by the harness. */
seq_t a(idx_t n)
{
    /* Initialise the cache the first time the function is called. */
    if (_a_cache.data == NULL) {
        _a_cache.size = 4;                                 /* initial capacity */
        _a_cache.data = (seq_t*)calloc(_a_cache.size, sizeof(seq_t));
        _a_cache.data[0] = 0;   /* a(0) */
        _a_cache.data[1] = 0;   /* a(1) */
        _a_cache.data[2] = 1;   /* a(2) */
        _a_cache.computed_to = 2;
    }

    /* Grow the cache if it does not yet hold an entry for n. */
    if (n >= _a_cache.size) {
        idx_t new_size = _a_cache.size;
        while (new_size <= n) new_size <<= 1;
        if (new_size == 0) new_size = 1;
        _a_cache.data = (seq_t*)realloc(_a_cache.data, new_size * sizeof(seq_t));
        for (idx_t i = _a_cache.size; i < new_size; ++i)
            _a_cache.data[i] = 0;
        _a_cache.size = new_size;
    }

    /* Compute missing values up to n. */
    for (idx_t i = _a_cache.computed_to + 1; i <= n; ++i) {

        if (i <= 2) {                /* base cases already set */
            continue;
        }

        /* The recurrence is:
           a(i) = ((i*i - 2*i - 1)*a(i-1) + (i-1)*i*a(i-2)) / (i-2)   */
        if (sizeof(seq_t) <= 8) {
            /* For 64‑bit seq_t use a 128‑bit intermediate to avoid overflow. */
            unsigned __int128 coeff1 = (unsigned __int128)(i * i) - (unsigned __int128)(2 * (unsigned __int128)i) - 1;
            unsigned __int128 coeff2 = (unsigned __int128)(i - 1) * i;
            unsigned __int128 term1  = coeff1 * (unsigned __int128)_a_cache.data[i-1];
            unsigned __int128 term2  = coeff2 * (unsigned __int128)_a_cache.data[i-2];
            unsigned __int128 sum    = term1 + term2;
            _a_cache.data[i] = (seq_t)(sum / (unsigned __int128)(i - 2));
        } else {
            /* For larger seq_t the compiler’s big‑int arithmetic handles
               the necessary bits. */
            seq_t coeff1 = (seq_t)((idx_t)(i * i) - (idx_t)(2 * i) - 1);
            seq_t coeff2 = (seq_t)((idx_t)(i - 1) * i);
            seq_t term1  = coeff1 * _a_cache.data[i-1];
            seq_t term2  = coeff2 * _a_cache.data[i-2];
            _a_cache.data[i] = (term1 + term2) / (i - 2);
        }
    }

    _a_cache.computed_to = n;
    return _a_cache.data[n];
}
