/*
 * A099730  Array read by antidiagonals. Rows contain odd numbers reaching
 *          same odd successor in Collatz function iteration.
 * https://oeis.org/A099730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) reads the array T(n,k) given by T(n,k) = 2^(2*k+1) * (n +
 *            floor((n+1)/3)) + (4^(k+1)-1)/3 where the array is indexed with
 *            n starting at 0 and k ≥ 0, and a(n) is obtained by reading this
 *            array along anti‑diagonals, i.e. a(1)=T(0,0), a(2)=T(0,1),
 *            a(3)=T(1,0), etc. The function below implements...
 * Range:     u8 n <= 6, u16 n <= 28, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiply a seq_t by 2 iteratively; works for both 64‑bit and
   arbitrary‑precision seq_t types because the multiplication operator
   is overloaded for seq_t in the BOM. */
static seq_t pow2_seq_t(idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= 2;
    return res;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; convert to zero‑based index */
    idx_t idx = n - 1;

    /* Find the anti‑diagonal d such that idx lies in it.
       The cumulative count of elements up to diagonal d (inclusive) is
       (d+1)*(d+2)/2.
       We find the minimal d with (d+1)*(d+2)/2 > idx. */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= idx)
        ++d;

    /* Number of elements before this diagonal */
    idx_t prev = d * (d + 1) / 2;

    /* Position p within the diagonal (0‑based), k decreases from d to 0 */
    idx_t p = idx - prev;          /* 0 … d   */
    idx_t row = p;                 /* n = row (zero‑based) */
    idx_t k   = d - p;             /* column index */

    idx_t g = (row + 1) / 3;       /* floor((n+1)/3) */

    /* Compute the two terms of T(n,k) */
    seq_t term1 = pow2_seq_t(2 * k + 1) * ((seq_t)row + (seq_t)g);
    seq_t term2 = (pow2_seq_t(2 * k + 2) - (seq_t)1) / (seq_t)3;

    return term1 + term2;
}
