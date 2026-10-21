/*
 * A208309  Number of n X 3 0..1 arrays with new values 0..1 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward neighbors.
 * https://oeis.org/A208309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2) with a(1)=4 and a(2)=18
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the n-th term of OEIS sequence A208309.
   The function keeps a static cache so that calls with any order of n
   are answered in O(1) after the initial computation. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* not defined for n < 1 */

    static seq_t *cache = NULL;
    static size_t capacity = 0;   /* allocated size (indices 0..capacity-1) */
    static idx_t last = 0;        /* highest index computed so far */

    /* Allocate or grow the cache if necessary. */
    if (capacity <= (size_t)n) {
        size_t new_cap = capacity ? capacity * 2 : 4;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();      /* out‑of‑memory */
        cache = tmp;
        capacity = new_cap + 1; /* indices 0..new_cap */
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)          val = 4;
        else if (i == 2)     val = 18;
        else                 val = 5 * cache[i - 1] - 3 * cache[i - 2];
        cache[i] = val;
    }
    last = n;
    return cache[n];
}
