/*
 * A041893  Denominators of continued fraction convergents to sqrt(468).
 * https://oeis.org/A041893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1298*a(n-8) – a(n-16); a(0..15) are the given initial
 *            values
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 25, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Initial 16 terms. */
static const seq_t init_vals[16] = {
    (seq_t)1,  (seq_t)1,  (seq_t)2,  (seq_t)3,  (seq_t)8,
    (seq_t)11, (seq_t)19, (seq_t)30, (seq_t)1279,
    (seq_t)1309,(seq_t)2588,(seq_t)3897,(seq_t)10382,
    (seq_t)14279,(seq_t)24661,(seq_t)38940
};

#ifndef __cplusplus
/* C: dynamic cache stored in raw memory. */
static seq_t *cache_ptr = NULL;
static size_t cache_len = 0;
#endif

/* Main sequence routine. */
seq_t a(idx_t n)
{
    size_t ni = (size_t)n;

#if defined(__cplusplus)
    /* C++: use std::vector to manage object construction/destruction. */
    static std::vector<seq_t> cache_vec;
    size_t old_size = cache_vec.size();

    /* Ensure the first 16 values are available. */
    if (old_size < 16) {
        cache_vec.resize(16);
        for (size_t i = 0; i < 16; ++i)
            cache_vec[i] = init_vals[i];
        old_size = 16;
    }

    /* Compute any missing values up to ni. */
    if (old_size <= ni) {
        cache_vec.resize(ni + 1);
        for (size_t i = old_size; i <= ni; ++i)
            cache_vec[i] = (seq_t)1298 * cache_vec[i - 8] - cache_vec[i - 16];
    }
    return cache_vec[ni];

#else
    /* C implementation: raw memory cache. */
    size_t old_len = cache_len;

    if (ni + 1 > cache_len) {
        size_t needed = ni + 1;
        seq_t *tmp = (seq_t *)realloc(cache_ptr, needed * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;          /* out of memory */
        cache_ptr = tmp;

        /* Initialise first 16 elements if we weren't large enough. */
        if (old_len < 16) {
            size_t upto = 16;
            if (needed < upto)
                upto = needed;
            for (size_t i = old_len; i < upto; ++i)
                cache_ptr[i] = init_vals[i];
        }
        cache_len = needed;
    }

    /* Compute new terms, starting from the first index not yet computed. */
    size_t start = old_len;
    if (start < 16)
        start = 16;
    for (size_t i = start; i <= ni; ++i)
        cache_ptr[i] = (seq_t)1298 * cache_ptr[i - 8] - cache_ptr[i - 16];

    return cache_ptr[ni];
#endif
}
