/*
 * A126326  a(1) = 1; for n>1, a(n) = smallest number which is not a sum or
 *          product or power of any subset of the numbers a(1) to a(n-1).
 * https://oeis.org/A126326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer which is not a sum, product or
 *            power of any subset of a(1) … a(n-1). For n > 16 the sequence
 *            satisfies the linear recurrence a(n) = 2*a(n-2) + 4*a(n-4) –
 *            2*a(n-6). a(1) = 1, a(2) = 2, a(3) = 5, a(4) = 9, a(5) = 13,
 *            a(6) = 31, a(7) = 35, a(8) = 92, a(9) = 118, a(10...
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* for size_t */
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

/* --- hard‑coded initial part of the sequence --- */
static const seq_t a_base[17] = {
    0,          /* dummy to make index 1 match the OEIS offset */
    1,          /* a(1) */
    2,          /* a(2) */
    5,          /* a(3) */
    9,          /* a(4) */
    13,         /* a(5) */
    31,         /* a(6) */
    35,         /* a(7) */
    92,         /* a(8) */
    118,        /* a(9) */
    280,        /* a(10) */
    516,        /* a(11) */
    752,        /* a(12) */
    1618,       /* a(13) */
    1968,       /* a(14) */
    5090,       /* a(15) */
    6594        /* a(16) */
};

/* --- static cache of computed terms --- */
static seq_t *a_cache = NULL;   /* array indexed 0..len */
static idx_t  a_cache_len = 0;  /* highest index for which a_cache is defined */

/* Grow the cache to hold terms up to new_len (inclusive) */
static void ensure_a_cache(idx_t new_len)
{
    if (new_len <= a_cache_len)
        return;                       /* nothing to do */

    /* Allocate a new array with enough space */
    seq_t *new_arr = seq_alloc_array((size_t)new_len + 1);
    if (a_cache != NULL) {
        /* Copy old values into new array */
        for (idx_t i = 0; i <= a_cache_len; ++i)
            new_arr[i] = a_cache[i];
        seq_free_array(a_cache);
    }
    a_cache = new_arr;

    /* Compute missing entries */
    for (idx_t i = a_cache_len + 1; i <= new_len; ++i) {
        if (i <= 16) {
            a_cache[i] = a_base[i];
        } else {
            /* recurrence: a(i) = 2*a(i-2) + 4*a(i-4) – 2*a(i-6) */
            a_cache[i] = 2 * a_cache[i - 2]
                       + 4 * a_cache[i - 4]
                       - 2 * a_cache[i - 6];
        }
    }

    a_cache_len = new_len;
}

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* safety for out‑of‑range call */

    ensure_a_cache(n);
    return a_cache[n];
}
