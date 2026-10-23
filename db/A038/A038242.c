/*
 * A038242  Triangle whose (i,j)-th entry is binomial(i,j)*4^(i-j)*12^j.
 * https://oeis.org/A038242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 4^i * 3^j where i = ⌊(√(8n + 1) – 1)/2⌋
 *            and j = n – i*(i+1)/2
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, the triangle entry T(i,j) = C(i,j)·4^(i-j)·12^j = C(i,j)·4^i·3^j,
   enumerated row‑wise with offset 0. */

#include "../utils/binomial.h"
#include "../utils/pow4.h"
#include "../utils/pow3.h"

/* Compute the row (i) and column (j) for a zero‑based linear index n.
   The row contains i+1 entries.  The cumulative count up to row i is
   (i+1)(i+2)/2.  Find the smallest i such that that count > n. */
static inline idx_t row_from_index(idx_t n)
{
    idx_t i = 0;
    /* The loop is O(√n); for the typical n≤10 000 it runs in <200 steps. */
    while ((i + 1) * (i + 2) / 2 <= n)
        ++i;
    return i;
}

seq_t a(idx_t n)
{
    idx_t i = row_from_index(n);
    idx_t prev_sum = i * (i + 1) / 2;   /* entries before row i */
    idx_t j = n - prev_sum;            /* column in row i */

    seq_t val = binomial(i, j);        /* C(i,j) */
    val *= pow4(i);                    /* 4^i */
    val *= pow3(j);                    /* 3^j = 12^j / 4^j */
    return val;
}
