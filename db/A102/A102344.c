/*
 * A102344  Numbers n such that the Diophantine equation (x+2)^3-x^3=2*n^2 has
 *          solutions.
 * https://oeis.org/A102344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(1) = 2
 *   a(2) = 7
 *   a(3) = 97
 *   a(n) = 14*a(n-1) - a(n-2)   for n >= 4   (equivalently a(n+2)=14*a(n+1)-a(n) for n≥2) */
#include <stdlib.h>

static seq_t *tbl = NULL;   /* 1‑based table of computed terms */
static idx_t  tbl_len = 0;  /* largest index that has been computed */

/* grow the table so that indices up to n are available */
static void grow_to(idx_t n)
{
    if (tbl_len >= n)
        return;                /* already have the requested terms */

    idx_t new_len = n + 1;      /* 0 unused, indices 1..n valid */

    tbl = (seq_t *)realloc(tbl, sizeof(seq_t) * (size_t)new_len);

    /* compute missing terms in increasing order */
    for (idx_t i = tbl_len + 1; i <= n; ++i)
    {
        if (i == 1)
            tbl[i] = (seq_t)2;
        else if (i == 2)
            tbl[i] = (seq_t)7;
        else if (i == 3)
            tbl[i] = (seq_t)97;
        else
            tbl[i] = (seq_t)14 * tbl[i - 1] - tbl[i - 2];
    }

    tbl_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* outside offset – return 0 for safety */

    grow_to(n);
    return tbl[n];
}
