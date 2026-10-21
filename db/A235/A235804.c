/*
 * A235804  Rectangular array read by upward antidiagonals: A(n,k) =
 *          n-2+k*2^(n-3), n>=3, k>=0.
 * https://oeis.org/A235804
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value read by upward antidiagonals of * the
 *            two–dimensional array A(n,k) = n-2 + k*2^(n-3), * with n>=3,
 *            k>=0. The sequence index n refers to the * OEIS offset 3: a(3) =
 *            first term
 * Range:     u8 n <= 48, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   find_d, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/find_d.h"

seq_t
a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;          /* values below the offset are undefined */

    idx_t m   = n - 3;                /* zero‑based position in the sequence */

    /* Determine diagonal number d and offset within that diagonal */
    idx_t d     = find_d(m);
    idx_t start = (idx_t)(((__int128)d * (d + 1)) / 2);  /* first index of diag d */
    idx_t offset = m - start;          /* k value, 0 <= offset <= d */

    idx_t k   = offset;
    idx_t L   = d - k;                 /* n-3 */
    idx_t row = d - k + 3;             /* actual row index n */

    seq_t pow   = pow2(L);              /* 2^(n-3) */
    seq_t val   = (seq_t)(row - 2) + (seq_t)k * pow;

    return val;
}
