/*
 * A103944  Number of rooted unicursal n-edge maps in the plane (planar with a
 *          distinguished outside face).
 * https://oeis.org/A103944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1, a(2) = 10, a(3) = 93
   For n ≥ 4:
     (n‑1) * a(n) = 3*(3n‑4) * a(n‑1)
                   − 6*(n‑9) * a(n‑2)
                   − 8*(2n‑5) * a(n‑3)   */

/* This implementation memoises the values in a dynamic array so that
   calls in any order are handled efficiently.  The recurrence is
   rearranged to avoid negative intermediate results:  the term
   involving a(n‑2) is added when n<9 (since 6*(n‑9) is negative) and
   subtracted otherwise.  The term involving a(n‑3) is always subtracted. */
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static storage for memoised values */
    static seq_t *table   = NULL;      /* dynamically allocated array */
    static idx_t  capacity = 0;        /* total allocated size (incl. index 0) */
    static idx_t  size     = 0;        /* actual number of filled entries +1 */

    /* Ensure capacity for index n (plus dummy index 0) */
    if (capacity < n + 1) {
        idx_t new_cap = capacity ? capacity << 1 : 4;
        while (new_cap < n + 1) new_cap <<= 1;
        seq_t *tmp = seq_realloc(table, new_cap);
        table    = tmp;
        capacity = new_cap;
        /* initialise new area to 0 (not strictly required for unsigned types) */
    }

    /* Base case: initialise first terms once */
    if (size == 0) {
        table[0] = (seq_t)0;            /* unused but harmless */
        table[1] = (seq_t)1;            /* a(1) */
        size = 2;
        if (n == 1) return table[1];
    }
    if (size <= 2 && n >= 2) {
        table[2] = (seq_t)10;           /* a(2) */
        size = 3;
        if (n == 2) return table[2];
    }
    if (size <= 3 && n >= 3) {
        table[3] = (seq_t)93;           /* a(3) */
        size = 4;
        if (n == 3) return table[3];
    }

    /* Compute missing values up to n using the recurrence */
    for (idx_t i = size; i <= n; ++i) {
        if (i == 1) { table[1] = (seq_t)1; }
        else if (i == 2) { table[2] = (seq_t)10; }
        else if (i == 3) { table[3] = (seq_t)93; }
        else {
            /* terms of the recurrence */
            seq_t t1 = table[i-1] * (seq_t)(3 * (3*i - 4));

            /* term involving a(i-2) */
            if (i < 9) {
                seq_t add = table[i-2] * (seq_t)(6 * (9-i));
                t1 += add;
            } else {
                seq_t sub = table[i-2] * (seq_t)(6 * (i-9));
                t1 -= sub;
            }

            /* term involving a(i-3) (always subtracted) */
            seq_t sub3 = table[i-3] * (seq_t)(8 * (2*i - 5));
            t1 -= sub3;

            /* divide by (i-1) */
            seq_t div = (seq_t)(i-1);
            table[i] = t1 / div;
        }
    }

    size = n + 1;
    return table[n];
}
