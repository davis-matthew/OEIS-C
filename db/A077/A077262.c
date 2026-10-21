/*
 * A077262  Second member of the Diophantine pair (m,k) that satisfies 5*(m^2
 *          + m) = k^2 + k; a(n) = k.
 * https://oeis.org/A077262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 18*a(n-2) - a(n-4) + 8 (n ≥ 4) Initial values: a(0)=0,
 *            a(1)=5, a(2)=14, a(3)=99 (The recurrence is valid for all n ≥ 4
 *            and generates the entire sequence.)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static table with memoised values */
    static seq_t *tbl = NULL;     /* array of computed terms */
    static size_t cap = 0;        /* allocated capacity (in terms of table entries) */
    static idx_t last = 0;        /* number of already computed terms (next free index) */

    /* -------------------------------------------------------------
       1. Initialise memoisation storage if needed
    ------------------------------------------------------------- */
    if (last == 0) {
        /* start with a reasonable initial capacity */
        cap = 16;
        tbl = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!tbl) return 0;      /* allocation failure: return 0 as a safe fallback */
    }

    /* -------------------------------------------------------------
       2. Grow the table if the requested index exceeds current capacity
    ------------------------------------------------------------- */
    if (n >= cap) {
        size_t newcap = cap ? cap << 1 : 16;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(tbl, newcap * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failure */
        tbl = tmp;
        cap = newcap;
    }

    /* -------------------------------------------------------------
       3. Ensure base values a(0) … a(3) are set
    ------------------------------------------------------------- */
    if (last < 4) {
        /* set the first four terms unconditionally (does not hurt when n < 3) */
        tbl[0] = (seq_t)0;
        tbl[1] = (seq_t)5;
        tbl[2] = (seq_t)14;
        tbl[3] = (seq_t)99;

        /* `last` becomes the count of terms that are guaranteed to be valid */
        /* if the caller asked for less than 4, we only claim the needed amount */
        last = (n < 3) ? (idx_t)n + 1 : (idx_t)4;
    }

    /* -------------------------------------------------------------
       4. Compute missing terms up to n (if any)
    ------------------------------------------------------------- */
    for (idx_t i = last; i <= n; ++i) {
        /* recurrence: a(i) = 18*a(i-2) - a(i-4) + 8  (i ≥ 4) */
        tbl[i] = ((seq_t)18 * tbl[i - 2]) - tbl[i - 4] + (seq_t)8;
    }
    last = n + 1; /* we now have values up to index n */

    return tbl[n];
}
