/*
 * A102371  Numbers missing from A102370.
 * https://oeis.org/A102371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1; for n > 1, a(n) = a(n-1) XOR (a(n-1) + n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for memoization */
    static seq_t *tab = NULL;   /* 1-indexed array: tab[1] = a(1), ... */
    static idx_t allocated = 0; /* maximum index currently allocated */
    static idx_t max_computed = 0; /* largest n for which a(n) has been computed */

    if (n == 0) return 0; /* undefined for the sequence, guard */

    /* grow storage if necessary */
    if (n > allocated) {
        idx_t new_alloc = allocated ? allocated * 2 : 128;
        while (new_alloc < n) new_alloc *= 2;
        seq_t *tmp = (seq_t *)realloc(tab, (new_alloc + 1) * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure (unlikely) */
        tab = tmp;
        allocated = new_alloc;
    }

    /* base case */
    if (max_computed == 0) {
        tab[1] = 1;                /* a(1) */
        max_computed = 1;
    }

    /* compute missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t prev = tab[i - 1];
        tab[i] = prev ^ (prev + (seq_t)i);
    }
    max_computed = n;

    return tab[n];
}
