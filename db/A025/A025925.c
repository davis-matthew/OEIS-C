/*
 * A025925  Expansion of 1/((1-x^9)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A025925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A025925: Number of partitions of n into parts 9, 11 and 12.
   Recurrence:
     a(0) = 1,
     a(n) = 0 for 1 ≤ n < 9,
     a(n) = a(n-9) + a(n-11) + a(n-12)
           - a(n-20) - a(n-21) - a(n-23) + a(n-32)    (n ≥ 0)          */

#include <stdlib.h>

/* Computes a(n) for the OEIS sequence A025925.
   The implementation memoises values up to the
   largest index requested so far and uses the linear recurrence above. */
seq_t a(idx_t n)
{
    /* Special case for the offset */
    if (n == 0) return (seq_t)1;

    /* Static state for memoisation */
    static seq_t *tab      = NULL; /* memoised values */
    static idx_t size      = 0;    /* allocated array size   */
    static idx_t calc_up_to = 0;    /* highest index already computed */

    /* Ensure the array has room for n+1 entries */
    if (size <= n) {
        idx_t new_size = n + 1;
        /* cast the void* from realloc to seq_t* so the code compiles in C++ */
        seq_t *tmp = (seq_t *)realloc(tab, new_size * sizeof(seq_t));
        if (!tmp) return 0;                   /* allocation failure */
        tab = tmp;
        /* initialise new slots to 0 */
        for (idx_t i = size; i < new_size; ++i) tab[i] = 0;
        size = new_size;
    }

    /* Initialise base case */
    if (tab[0] == 0) tab[0] = (seq_t)1;

    /* Compute all values from calc_up_to+1 up to n */
    for (idx_t k = calc_up_to + 1; k <= n; ++k) {
        /* Sum of positive terms a(k-9), a(k-11), a(k-12) */
        seq_t pos = 0;
        if (k >= 9)  pos += tab[k - 9];
        if (k >= 11) pos += tab[k - 11];
        if (k >= 12) pos += tab[k - 12];

        /* Optional positive term a(k-32) */
        seq_t add = (k >= 32) ? tab[k - 32] : (seq_t)0;

        /* Sum of negative terms a(k-20), a(k-21), a(k-23) */
        seq_t neg = 0;
        if (k >= 20) neg += tab[k - 20];
        if (k >= 21) neg += tab[k - 21];
        if (k >= 23) neg += tab[k - 23];

        /* Compute the value: (pos + add) - neg */
        seq_t val = pos + add;
        val -= neg;
        tab[k] = val;
    }
    calc_up_to = n;

    return tab[n];
}
