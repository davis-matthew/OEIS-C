/*
 * A196875  a(n) = a(n-4) + a(n-3) + a(n-2) + a(n-1) + (n-5).
 * https://oeis.org/A196875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 36, u64 n <= 70, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A196875
 *
 * Recurrence:   a(1)=a(2)=a(3)=a(4)=1
 *               a(n)=a(n‑4)+a(n‑3)+a(n‑2)+a(n‑1)+(n‑5)   for n≥5
 * This function caches all terms computed so far so that calls in
 * arbitrary order are O(1) after the first calculation for a given n.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache: index 0 is unused so that the array is 1‑based */
    static seq_t *cache = NULL;   /* array of terms 0..max_n   */
    static size_t cap   = 0;      /* bytes allocated for cache */
    static idx_t  last  = 0;      /* highest n that has been computed */

    if (n == 0)                  /* undefined for n<1, return 0 for safety */
        return 0;

    /* Allocate or grow the cache if we need to compute larger terms. */
    if (last < n) {
        size_t new_cap = (size_t)n + 1;          /* need room for indices 0..n */
        if (cap < new_cap) {
            /* realloc returns void*; cast for both C++ and C. */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();                   /* allocation failed */
            cache = tmp;
            cap   = new_cap;
        }

        /* Compute terms from last+1 up to n */
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t val;
            if (i <= 4)
                val = 1;                         /* initial values */
            else
                val = cache[i-4] + cache[i-3] + cache[i-2] + cache[i-1]
                    + ((seq_t)(i - 5));
            cache[i] = val;
        }
        last = n;
    }

    return cache[n];
}
