/*
 * A057145  Square array of polygonal numbers T(n,k) = ((n-2)*k^2 -
 *          (n-4)*k)/2, n >= 2, k >= 1, read by antidiagonals upwards.
 * https://oeis.org/A057145
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice, tabl, easy
 * Formula:   a(n) is the n‑th term (offset 2) of the triangle * T(n,k) =
 *            ((n‑2)·k² – (n‑4)·k)/2 read by antidiagonals upwards
 * Range:     u8 n <= 129, u16 n <= 4716, u32 n <= 4951, u64 n <= 4951,
 *            u128 n <= 4951, big any n
 * Helpers:   T_formula
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/T_formula.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* undefined for n < 2, behaviour irrelevant */

    /* Zero‑based position in the antidiagonal ordering */
    idx_t idx = n - 2;

    /* Find smallest integer t ≥ 1 with t(t‑1)/2 > idx.
       Here t = s‑1 where s = n_row + n_col. */
    long double disc = sqrtl((long double)(1.0L + 8.0L * (long double)idx));
    idx_t t = (idx_t)((1.0L + disc) / 2.0L);   /* floor value */

    /* Adjust to satisfy the inequality (may increase by at most 1). */
    while ((__int128)t * (t - 1) / 2 <= idx) t++;

    idx_t s = t + 1;                /* sum of coordinates: n_row + n_col */
    idx_t cum_prev = (idx_t)(((__int128)(t - 1) * (t - 2)) / 2); /* elements before this diagonal */
    idx_t offset = idx - cum_prev;  /* position within the current diagonal, 0‑based */

    idx_t n_row = s - 1 - offset;   /* row index (n ≥ 2) */
    idx_t n_col = s - n_row;        /* column index (k ≥ 1) */

    return T_formula(n_row, n_col);
}
