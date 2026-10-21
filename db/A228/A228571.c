/*
 * A228571  The backwards antidiagonal sums of triangle A228570.
 * https://oeis.org/A228571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A228571: The backwards antidiagonal sums of triangle A228570
 *
 *  The sequence is defined by
 *
 *      a(n) = ∑_{k=0..⌊n/2⌋} A228570(n‑k, n‑2k)
 *
 *  It satisfies the generating function
 *
 *      G(x) = ½*(1/(1-x²-x⁵) + (1+x²+x⁵)/(1-x⁴-x¹⁰)) .
 *
 *  Using the recurrences for the two series
 *
 *      g1(n) = g1(n-2) + g1(n-5),  g1(0)=1, g1(1)=0
 *      h(n)  = h(n-4)  + h(n-10), h(0)=1 , h(n<0)=0
 *
 *  let
 *
 *      g2(n) = h(n) + h(n-2) + h(n-5)    (coefficients of (1+x²+x⁵)/(1-x⁴-x¹⁰))
 *
 *  then  a(n) = (g1(n) + g2(n))/2 .
 *
 *  The implementation keeps the three coefficient arrays memoised so that
 *  each call is O(n) time and O(n) memory for the largest n requested.
 *
 *  The code is valid C11 and C++17.  Memory is allocated with malloc;
 *  the cast to (seq_t *) is required for C++.
 */

#include <stdlib.h>

static seq_t *g1 = NULL;   /* coefficients of 1/(1-x²-x⁵)   */
static seq_t *h  = NULL;   /* coefficients of 1/(1-x⁴-x¹⁰)  */
static seq_t *g2 = NULL;   /* sum of h(n),h(n-2),h(n-5)     */
static idx_t  max_n = 0;   /* largest n that has been computed */

/* Allocate or extend the coefficient arrays up to index n (inclusive). */
static void ensure_computed(idx_t n)
{
    size_t new_sz = (size_t)n + 1;

    if (g1 == NULL)            /* first call: allocate all arrays */
    {
        g1 = (seq_t *)malloc(new_sz * sizeof(seq_t));
        h  = (seq_t *)malloc(new_sz * sizeof(seq_t));
        g2 = (seq_t *)malloc(new_sz * sizeof(seq_t));
        if (g1 == NULL || h == NULL || g2 == NULL)
            return;           /* memory allocation failed – give up */

        for (idx_t i = 0; i <= n; ++i)
        {
            /* g1 computation */
            if (i == 0)            g1[i] = 1;   /* g1(0)=1      */
            else if (i == 1)        g1[i] = 0;   /* g1(1)=0      */
            else
                g1[i] = (i >= 2 ? g1[i-2] : 0) + (i >= 5 ? g1[i-5] : 0);

            /* h computation */
            if (i == 0)            h[i] = 1;   /* h(0)=1       */
            else if (i == 1)        h[i] = 0;   /* h(1)=0 (default) */
            else
                h[i] = (i >= 4 ? h[i-4] : 0) + (i >= 10 ? h[i-10] : 0);

            /* g2 computation */
            g2[i] = h[i] +
                    (i >= 2 ? h[i-2] : 0) +
                    (i >= 5 ? h[i-5] : 0);
        }
        max_n = n;
    }
    else if (n > max_n)        /* extend arrays to new size */
    {
        g1 = (seq_t *)realloc(g1, new_sz * sizeof(seq_t));
        h  = (seq_t *)realloc(h , new_sz * sizeof(seq_t));
        g2 = (seq_t *)realloc(g2, new_sz * sizeof(seq_t));
        if (g1 == NULL || h == NULL || g2 == NULL)
            return;           /* reallocation failed – give up */

        for (idx_t i = max_n + 1; i <= n; ++i)
        {
            /* g1 computation */
            if (i == 0)            g1[i] = 1;
            else if (i == 1)        g1[i] = 0;
            else
                g1[i] = (i >= 2 ? g1[i-2] : 0) + (i >= 5 ? g1[i-5] : 0);

            /* h computation */
            if (i == 0)            h[i] = 1;
            else if (i == 1)        h[i] = 0;
            else
                h[i] = (i >= 4 ? h[i-4] : 0) + (i >= 10 ? h[i-10] : 0);

            /* g2 computation */
            g2[i] = h[i] +
                    (i >= 2 ? h[i-2] : 0) +
                    (i >= 5 ? h[i-5] : 0);
        }
        max_n = n;
    }
}

/* ------------------------------------------------------------------------ *
 *  public function
 * ------------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    ensure_computed(n);
    seq_t sum = g1[n] + g2[n];
    return sum / 2;   /* the sum is always even */
}
