/*
 * A103944  Number of rooted unicursal n-edge maps in the plane (planar with a
 *          distinguished outside face).
 * https://oeis.org/A103944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 194 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

/* Recurrence:
   a_wide(1) = 1, a_wide(2) = 10, a_wide(3) = 93
   For n ≥ 4:
     (n‑1) * a_wide(n) = 3*(3n‑4) * a_wide(n‑1)
                   − 6*(n‑9) * a_wide(n‑2)
                   − 8*(2n‑5) * a_wide(n‑3)   */

/* This implementation memoises the values in a dynamic array so that
   calls in any order are handled efficiently.  The recurrence is
   rearranged to avoid negative intermediate results:  the term
   involving a_wide(n‑2) is added when n<9 (since 6*(n‑9) is negative) and
   subtracted otherwise.  The term involving a_wide(n‑3) is always subtracted. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static inline seq_t *seq_realloc(seq_t *p, idx_t n)
{
#ifdef __cplusplus
    return mem_resize(seq_t, p, n);
#else
    return (seq_t *)realloc(p, n * sizeof(seq_t));
#endif
}

seq_t a_wide(idx_t n)
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
        table[1] = (seq_t)1;            /* a_wide(1) */
        size = 2;
        if (n == 1) return table[1];
    }
    if (size <= 2 && n >= 2) {
        table[2] = (seq_t)10;           /* a_wide(2) */
        size = 3;
        if (n == 2) return table[2];
    }
    if (size <= 3 && n >= 3) {
        table[3] = (seq_t)93;           /* a_wide(3) */
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

            /* term involving a_wide(i-2) */
            if (i < 9) {
                seq_t add = table[i-2] * (seq_t)(6 * (9-i));
                t1 += add;
            } else {
                seq_t sub = table[i-2] * (seq_t)(6 * (i-9));
                t1 -= sub;
            }

            /* term involving a_wide(i-3) (always subtracted) */
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

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
