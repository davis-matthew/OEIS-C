/*
 * A219021  Sum of cubes of first n terms of Lucas sequence U(4,1) (A001353)
 *          divided by sum of their first powers.
 * https://oeis.org/A219021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} A001353(k)^3 / Sum_{k=1..n} A001353(k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*  Lucas sequence U(4,1):
    U_0 = 0, U_1 = 1,
    U_n = 4*U_{n-1} - U_{n-2}
*/

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;      /* a[0] corresponds to a(1) */
    static idx_t cache_len = 0;      /* number of stored terms */
    static idx_t cache_cap = 0;      /* allocated capacity */

    /* If no cache yet, compute the first 5 terms */
    if (cache_len == 0)
    {
        /* Allocate space for at least 5 terms */
        cache_cap = 5;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));

        /* Compute U(4,1) terms and their sums up to the 5th term */
        seq_t sumU = 0;
        seq_t sumC = 0;
        seq_t u_prevprev = 0;   /* U_{k-2} */
        seq_t u_prev = 1;       /* U_{k-1}, U_1 = 1 */

        for (idx_t k = 1; k <= 5; ++k) {
            seq_t U_k = u_prev;                 /* current term */
            sumU += U_k;
            sumC += U_k * U_k * U_k;           /* U_k^3 */
            cache[k-1] = sumC / sumU;          /* a(k) */

            /* Prepare next Lucas term */
            seq_t u_next = 4 * u_prev - u_prevprev;
            u_prevprev = u_prev;
            u_prev = u_next;
        }

        cache_len = 5;
    }

    /* Return cached value if already computed */
    if (n <= cache_len)
        return cache[n-1];

    /* Extend the cache to accommodate term 'n' if needed */
    if (n > cache_cap) {
        cache_cap = (size_t)n;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Compute missing terms using the 5th‑order linear recurrence:
          a(n) = 19*a(n-1) - 76*a(n-2) + 76*a(n-3) - 19*a(n-4) + a(n-5)
    */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        /* Retrieve 5 previous values (indices are zero‑based) */
        seq_t a1 = cache[k-2]; /* a(k-1) */
        seq_t a2 = cache[k-3]; /* a(k-2) */
        seq_t a3 = cache[k-4]; /* a(k-3) */
        seq_t a4 = cache[k-5]; /* a(k-4) */
        seq_t a5 = cache[k-6]; /* a(k-5) */

        /* Compute big and small terms separately to avoid underflow */
        seq_t big   = a1 * 19 + a3 * 76 + a5;
        seq_t small = a2 * 76 + a4 * 19;

        seq_t val = big - small;                 /* a(k) */
        cache[k-1] = val;
    }

    cache_len = n;
    return cache[n-1];
}
