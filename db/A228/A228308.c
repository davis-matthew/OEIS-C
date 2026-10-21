/*
 * A228308  Triangle read by rows: T(n,k) (n>=2, 1<=k<=n-1) is the number of
 *          unordered pairs of vertices at distances k in the odd graph O_n.
 * https://oeis.org/A228308
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the (n-2)‑th element in the flattened triangle whose rows
 *            are the unordered pair counts T(n,k) for the odd graph O_n,
 *            sorted ascending within each row. T(n,k) = C(2n‑1, n‑1) * C(n‑1,
 *            k‑1) * C(n, k) / 2
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is read by rows starting with n = 2.  The first index n = 2
   yields the first element (3).  Subsequent indices n correspond to the
   subsequent elements of the sorted rows.                                */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"

/* Compare two seq_t values for qsort.  Works for built‑in integer types
   as well as for arbitrary‑precision integer classes that overload <.   */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    if (*va < *vb) return -1;
    if (*va > *vb) return 1;
    return 0;
}

/* Compute a single triangle entry T(n,k).  The division by 2 is performed
   by pre‑dividing one of the factors to avoid overflow for 64‑bit builds. */
static seq_t triangle_T(idx_t n, idx_t k)
{
    seq_t v = binomial(2*n-1, n-1);
    seq_t b = binomial(n-1, k-1);
    seq_t c = binomial(n, k);

    /* Divide one factor by 2 before multiplication to keep intermediate
       values within 64 bits, when seq_t is a 64‑bit integer. */
    if ((v & 1) == 0) {
        v /= 2;
    } else if ((b & 1) == 0) {
        b /= 2;
    } else {
        c /= 2;
    }

    seq_t term = v * b;
    term = term * c;
    return term;
}

/* Main sequence function.  The argument n is >= 2.  For n < 2 the
   function returns 0 (undefined behaviour for the OEIS sequence). */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                       /* safe guard */

    /* Position in the flattened triangle, starting from 0 at the first
       element (n = 2). */
    idx_t pos = n - 2;

    /* Find the row m such that the cumulative counts up to row m
       (i.e., (m-1)*m/2) exceed pos.  Row m has m-1 entries. */
    idx_t m = 2;
    while (((m-1) * m) / 2 <= pos) ++m;

    /* Offset within the current row (0‑based). */
    idx_t prev = (m-2) * (m-1) / 2;
    idx_t offset = pos - prev;                 /* 0 ≤ offset < m-1 */

    size_t len = (size_t)(m-1);
    seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
    if (!row) return 0;                        /* memory allocation failed */

    /* Compute all triangle entries for this row. */
    for (idx_t k = 1; k <= m-1; ++k) {
        row[k-1] = triangle_T(m, k);
    }

    /* Sort the row ascending. */
    qsort(row, len, sizeof(seq_t), cmp_seq_t);

    /* The required term is at position 'offset' in the sorted row. */
    seq_t result = row[offset];
    free(row);
    return result;
}
