/*
 * A194604  Square table T(n, d) read by antidiagonals: number of ways to
 *          place 2 nonattacking kings on an n^d (n X n X ...) raumschach
 *          board (hypercubical chessboard).
 * https://oeis.org/A194604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (n^(2d) - (3n-2)^d) / 2 The sequence is read by
 *            antidiagonals of the table T(n,d) = (n^(2d) - (3n-2)^d) / 2 for
 *            n>0,d>0. For each diagonal sum s=n+d (s≥2) we list the points in
 *            order of decreasing n: (s-1,1),(s-2,2),…,(1,s-1)
 * Range:     u8 n <= 12, u16 n <= 31, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t k)
{
    /* Find diagonal s such that C(s-1) < k ≤ C(s),
       where C(s) = (s-1)*s/2 is the number of terms
       up to and including diagonal s. */
    idx_t s = 2;
    while (((idx_t)((s - 1) * (unsigned long long)s) / 2) < k)
        ++s;

    idx_t C_prev = ((idx_t)((s - 2) * (unsigned long long)(s - 1)) / 2);
    idx_t offset = k - C_prev - 1;          /* 0‑based offset within diagonal */

    idx_t n_coord = (s - 1) - offset;       /* row (n) */
    idx_t d_coord = 1 + offset;             /* column (d) */

    /* Compute T(n_coord, d_coord) = (n^(2d) - (3n-2)^d) / 2 */

    seq_t n_val   = (seq_t)n_coord;
    seq_t d_val   = (seq_t)d_coord;

    seq_t exp1 = d_val * 2;                 /* 2*d */
    seq_t power1 = big_pow(n_val, exp1);    /* n^(2d) */

    seq_t base2 = (seq_t)3 * n_val - 2;     /* (3n-2) */
    seq_t power2 = big_pow(base2, d_val);   /* (3n-2)^d */

    seq_t diff = power1 - power2;           /* guaranteed even */
    seq_t result = diff / 2;

    return result;
}
