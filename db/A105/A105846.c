/*
 * A105846  Numbers n such that 41*n^2 + 41*n + 1 is a square.
 * https://oeis.org/A105846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A105846
   Numbers n such that 41*n^2 + 41*n + 1 is a square.
   Recurrence (n ≥ 5):
     a(1) = 0
     a(2) = 704
     a(3) = 4098*a(1) + 2048 - a(2)
     a(4) = 4098*a(2) + 2048 - a(1)
     a(n) = 4098*a(n-2) + 2048 - a(n-4)
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1u)
        return (seq_t)0;

    /* --- memoisation table --------------------------------- */
    static seq_t *tab = NULL;   /* array of terms, 1‑based */
    static size_t cap = 0;      /* maximum index that fits in the array */
    static idx_t  max = 0;      /* largest index that has been computed */

    /* initialise once */
    if (!tab) {
        cap   = 16u;                     /* first table size (max index) */
        tab   = (seq_t *)malloc((cap + 1) * sizeof(seq_t)); /* +1 for 1‑based use */
        tab[0] = 0;                      /* unused slot, just in case          */
        tab[1] = (seq_t)0;               /* a(1) */
        tab[2] = (seq_t)704;             /* a(2) */
        tab[3] = (seq_t)1344;            /* a(3) */
        tab[4] = (seq_t)2887040;         /* a(4) */
        max = 4;
    }

    /* fast return if already known */
    if (n <= max)
        return tab[n];

    /* grow table if needed */
    if ((idx_t)n > cap) {
        size_t newcap = cap;
        while ((idx_t)n > newcap) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(tab, (newcap + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /* allocation failure */
        tab = tmp;
        cap = newcap;
    }

    /* extend the sequence using the recurrence */
    for (idx_t i = max + 1; i <= n; ++i) {
        seq_t term_nm2 = tab[i - 2];
        seq_t term_nm4 = tab[i - 4];
        seq_t prod     = term_nm2 * (seq_t)4098;
        seq_t sum      = prod + (seq_t)2048;
        tab[i]         = sum - term_nm4;
    }
    max = n;

    return tab[n];
}
