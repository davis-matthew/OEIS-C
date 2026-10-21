/*
 * A095003  a(n) = 9*a(n-1) - 9*a(n-2) + a(n-3).
 * https://oeis.org/A095003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A095003: a(n) = 9*a(n-1) - 9*a(n-2) + a(n-3)
   with a(1)=1, a(2)=6, a(3)=45.  Offset: 1. */

#include <stdlib.h>   /* for malloc, realloc */

static seq_t *cache = NULL;      /* storage for computed terms */
static size_t cache_cap = 0;     /* current allocated capacity (in terms) */
static size_t cache_size = 0;    /* number of terms actually stored (>=0) */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n<1; return 0 */

    /* Ensure capacity to store up to n terms */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 3;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *p = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
        cache = p;
        cache_cap = new_cap;
    }

    /* Initialise the first three terms if not already done */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;          /* a(1) */
        if (cache_cap >= 2) cache[1] = (seq_t)6;      /* a(2) */
        if (cache_cap >= 3) cache[2] = (seq_t)45;     /* a(3) */
        cache_size = 3;
    }
    else if (cache_size < 3) {
        /* In case of partial pre‑initialisation, fill remaining */
        if (cache_size == 1 && cache_cap >= 2) cache[1] = (seq_t)6;
        if (cache_size <= 2 && cache_cap >= 3) cache[2] = (seq_t)45;
        cache_size = 3;
    }

    /* Compute missing terms up to n */
    if ((size_t)n > cache_size) {
        const seq_t nine = (seq_t)9;
        for (size_t i = cache_size; i < (size_t)n; ++i) {
            /* indices: i-1 -> a(i), i-2 -> a(i-1), i-3 -> a(i-2) */
            seq_t a1 = cache[i-1];   /* a(i)   */
            seq_t a2 = cache[i-2];   /* a(i-1) */
            seq_t a3 = cache[i-3];   /* a(i-2) */
            cache[i] = nine * a1 - nine * a2 + a3;
        }
        cache_size = (size_t)n;
    }

    return cache[n-1];  /* a(n) is stored at index n-1 */
}
