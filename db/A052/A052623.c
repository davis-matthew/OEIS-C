/*
 * A052623  E.g.f. x(1-x)^2/(1-3x+x^2).
 * https://oeis.org/A052623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A052623:  E.g.f.  x(1‑x)^2/(1‑3x+x^2)
    Recurrence (for n ≥ 2):
        a(n+2) = (6+3n)*a(n+1) – (n^2+3n+2)*a(n)
    Initial values:
        a(0)=0, a(1)=1, a(2)=2, a(3)=18
    The function memoizes results up to the largest requested index
    and grows its storage as needed.  All arithmetic uses the native
    seq_t type, so it works with arbitrary‑precision or fixed‑size
    unsigned types as provided by the build.  */

/* Include required standard headers for NULL, size_t, malloc and realloc */
#include <stddef.h>
#include <stdlib.h>

/* Return the n-th term of A052623.  */
seq_t a(idx_t n)
{
    /* Static table of terms, allocated on first use.  */
    static seq_t *tab = NULL;
    static idx_t last = 3;      /* last index currently stored (≥3)   */
    static size_t cap = 4;      /* allocated capacity (≥last+1)       */

    /* Allocate initial table with the first four terms.  */
    if (tab == NULL)
    {
        tab = (seq_t *)malloc(cap * sizeof(seq_t));
        if (tab == NULL)          /* out of memory: give up */
            return (seq_t)0;
        /* initial terms */
        tab[0] = (seq_t)0;
        tab[1] = (seq_t)1;
        tab[2] = (seq_t)2;
        tab[3] = (seq_t)18;
    }

    /* If n is already computed, simply return it.  */
    if (n <= last)
        return tab[n];

    /* Grow the table if necessary.  */
    if ((idx_t)(cap - 1) < n)
    {
        size_t new_cap = (size_t)n + 1;
        if (new_cap <= cap)
            new_cap = cap * 2;            /* ensure progress if n large */
        seq_t *new_tab = (seq_t *)realloc(tab, new_cap * sizeof(seq_t));
        if (new_tab == NULL)             /* realloc failed: give up */
            return (seq_t)0;
        tab = new_tab;
        cap = new_cap;
    }

    /* Compute missing terms using the recurrence.  */
    /* We need a(n+2) = (6+3n)*a(n+1) – (n^2+3n+2)*a(n)   for n ≥ 2 */
    for (idx_t i = last + 1; i <= n; ++i)    /* i is the index to compute */
    {
        /* Recurrence uses terms i-2 and i-1: */
        /* a(i) = (6+3*(i-2))*a(i-1) – ((i-2)^2+3*(i-2)+2)*a(i-2) */
        idx_t k = i - 2;                       /* current base index */
        seq_t coeff1 = (seq_t)(6 + 3 * k);         /* 6+3k */
        seq_t coeff2 = (seq_t)(k * k + 3 * k + 2); /* k^2+3k+2 */
        tab[i] = coeff1 * tab[i-1] - coeff2 * tab[i-2];
    }

    last = n;
    return tab[n];
}
