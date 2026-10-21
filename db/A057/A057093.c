/*
 * A057093  Scaled Chebyshev U-polynomials evaluated at i*sqrt(10)/2.
 *          Generalized Fibonacci sequence.
 * https://oeis.org/A057093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A057093: a(n) = 10*(a(n-1)+a(n-2)), a(-1)=0, a(0)=1. */
#include <stdlib.h>   /* malloc, realloc */

/* Return the n-th term (n ≥ 0).  The function is lazily memoised in a
   static array so that calls in any order remain efficient. */
seq_t a(idx_t n)
{
    /* Static cache of previously computed terms. */
    static seq_t *cache = NULL;   /* array of a(i) */
    static size_t  capacity = 0;  /* number of allocated slots */
    static size_t  length   = 0;  /* number of valid entries */

    /* Initialise cache if this is the first call.  a(0) = 1. */
    if (length == 0) {
        capacity = 1;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;       /* memory allocation failed */
        cache[0] = (seq_t)1;
        length = 1;
    }

    /* Ensure the array is large enough for index n. */
    size_t idx = (size_t)n;               /* n is uint64_t, fits into size_t on 64‑bit systems */
    if (idx >= capacity) {
        size_t newcap = capacity ? capacity : 1;
        while (newcap <= idx) newcap <<= 1;   /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* memory allocation failed */
        cache = tmp;
        capacity = newcap;
    }

    /* Compute any missing terms up to n. */
    if (idx >= length) {
        for (size_t i = length; i <= idx; ++i) {
            seq_t value;
            if (i == 1) {
                /* a(1) = 10 * (a(0) + a(-1)),  a(-1)=0. */
                value = (seq_t)10 * (cache[0] + (seq_t)0);
            } else {
                /* a(i) = 10*(a(i-1)+a(i-2))  for i ≥ 2. */
                value = (seq_t)10 * (cache[i-1] + cache[i-2]);
            }
            cache[i] = value;
        }
        length = idx + 1;
    }

    return cache[n];
}
