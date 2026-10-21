/*
 * A230929  Number of black-square subarrays of (n+2) X (2+2) 0..3 arrays
 *          x(i,j) with each element diagonally or antidiagonally next to at
 *          least one element with value (x(i,j)+1) mod 4, no adjacent
 *          elements equal, and upper left element zero.
 * https://oeis.org/A230929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2, a(2) = 6, a(3) = 16 a(n) = 3*a(n-1) - a(n-2) +
 *            3*a(n-3) for n >= 4
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function memoises the sequence to support any order of calls. */

#include <stdlib.h>   /* malloc, realloc */

/* Array cache for computed values (a(1) stored at cache[0]). */
static seq_t *cache   = NULL;  /* 0‑initialised automatically. */
static idx_t  cap     = 0;     /* capacity of the cache array.   */
static idx_t  len     = 0;     /* number of valid entries stored. */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 by caller; behaviour for n==0 is arbitrary. */
    if (n == 0) return 0;

    /* Initialise cache if first call. */
    if (!cache) {
        cap = 8;                       /* start with some space */
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        len = 0;
    }

    /* Grow cache if necessary. */
    if ((idx_t)n > cap) {
        size_t new_cap = cap;
        while ((idx_t)new_cap < (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cap   = new_cap;
    }

    /* Ensure base terms are stored. */
    if (len == 0) {
        cache[0] = (seq_t)2;   /* a(1) */
        cache[1] = (seq_t)6;   /* a(2) */
        cache[2] = (seq_t)16;  /* a(3) */
        len = 3;
    }

    /* Return if already computed. */
    if (n <= len) return cache[n-1];

    /* Compute missing values up to n. */
    for (idx_t i = len; i < n; ++i) {
        /* i is 0‑based index of a(i+1). Compute a(i+1) using recurrence. */
        seq_t term_prev   = cache[i-1];          /* a(i)   */
        seq_t term_prev2  = cache[i-2];          /* a(i-1) */
        seq_t term_prev3  = cache[i-3];          /* a(i-2) */
        seq_t term_mul1   = term_prev * 3;       /* 3*a(i) */
        seq_t term_mul3   = term_prev3 * 3;      /* 3*a(i-2) */
        cache[i] = term_mul1 + term_mul3 - term_prev2;
    }

    len = n;
    return cache[n-1];
}
