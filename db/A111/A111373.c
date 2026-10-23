/*
 * A111373  A generalized Pascal triangle.
 * https://oeis.org/A111373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(row_of(n), n - row_of(n)*(row_of(n)+1)/2) where the
 *            triangle entries T are given by T(n,n) = 1, T(n,k) = 0 if (n-k)
 *            mod 3 ≠ 0, T(n,k) = (3/(n-k)) * C(n, (n-k)/3-1) * (k+1)
 *            otherwise
 * Range:     u8 n <= 55, u16 n <= 302, u32 n <= 946, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binom, row_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/row_of.h"

seq_t a(idx_t n)
{
    idx_t r    = row_of(n);
    idx_t k    = n - r * (r + 1) / 2;   /* column within row r */
    if (k == r) return (seq_t)1;       /* diagonal entry */

    idx_t diff = r - k;
    if (diff % 3 != 0) return (seq_t)0;/* non‑zero diagonal only when divisible by 3 */

    idx_t m = diff / 3;                 /* m ≥ 1  */
    seq_t bin = binom(r, m - 1);        /* C(r, m-1) */

    seq_t value = bin * 3 * (k + 1);    /* (3 * bin * (k+1)) */
    value = value / diff;              /* divide by (r-k) */

    return value;
}
