/*
 * A191428  Dispersion of ([n*r+r]), where r=(golden ratio)=(1+sqrt(5))/2 and
 *          [ ]=floor, by antidiagonals.
 * https://oeis.org/A191428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the element at index n when the dispersion array of
 *            s(m)=⌊(m+1)·φ⌋ (φ=(1+√5)/2) is read by antidiagonals
 * Range:     u8 n <= 3, u16 n <= 36, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* helper: compute ⌊(k+1)·φ⌋ for a seq_t argument k  */
static seq_t floor_phi_mul(seq_t k)
{
    seq_t m   = k + 1;                     /* m = k+1 */
    seq_t m2  = m * m;                     /* m²   */
    seq_t t   = floor_sqrt(5 * m2);        /* ⌊√(5·m²)⌋ = ⌊(m)·√5⌋   */
    return (m + t) / 2;                    /* ⌊((m)+t)/2⌋ = ⌊(k+1)·φ⌋ */
}

/* helper: value of the complement sequence t(n) for n ≥ 1 */
static seq_t t_value(idx_t n)
{
    if (n == 1) return (seq_t)1;                    /* special case */
    seq_t k   = (seq_t)(n - 1);                     /* k = n-1 ≥ 1 */
    seq_t sp  = floor_phi_mul(k - 1);               /* ⌊k·φ⌋   */
    return k + sp;                                 /* k + ⌊k·φ⌋ = ⌊(k)·φ²⌋ */
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                          /* outside OEIS range */

    /* find diagonal d such that (d+1)(d+2)/2 ≥ n */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 < n) d++;

    /* number of elements before diagonal d */
    idx_t totalPrev = d * (d + 1) / 2;

    /* offset of n inside diagonal d (0‑based) */
    idx_t offset = n - totalPrev - 1;   /* 0 … d                */

    /* row (index in the dispersion array), 1‑based */
    idx_t row = offset + 1;

    /* column (step count), 1‑based */
    idx_t col = d + 2 - row;           /* 1 … d+1               */

    /* start value of the row: t(row) */
    seq_t val = t_value(row);

    /* apply s = ⌊(x+1)·φ⌋ (col‑1) times */
    for (idx_t k = 1; k < col; ++k)
        val = floor_phi_mul(val);

    return val;
}
