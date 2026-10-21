/*
 * A235803  Rectangular array read by upward antidiagonals: A(n,k) = 1 +
 *          sqrt(k)*((1+sqrt(k))^n - (1-sqrt(k))^n)/2, n,k >= 0.
 * https://oeis.org/A235803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A(i,k) where
 *   A(n,k) = 1 + Σ_{t=1}^{⌊(n+1)/2⌋} binom(n, 2t-1) * k^t
 *   (which equals 1 + sqrt(k)*((1+sqrt(k))^n-(1-sqrt(k))^n)/2).
 *
 *   For the sequence index n, let d be the unique non‑negative integer with
 *   d(d+1)/2 ≤ n < (d+1)(d+2)/2.
 *   Then i = d - (n - d(d+1)/2) and k = n - d(d+1)/2.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute diagonal d such that d(d+1)/2 <= n < (d+1)(d+2)/2 */
    idx_t d = 0;
    while ( ((unsigned __int128)(d + 1) * (d + 2) / 2) <= n)
        ++d;
    /* start index of this diagonal */
    idx_t start = (unsigned __int128)d * (d + 1) / 2;
    idx_t offset = n - start;      /* position inside the diagonal   */
    idx_t i = d - offset;          /* row index (n in formula)       */
    idx_t k = offset;              /* column index (k in formula)    */

    /* Simple special case: A(0,k) = 1 */
    if (i == 0)
        return (seq_t)1;

    seq_t sk = (seq_t)k;          /* cast k to the generalized integer type */

    seq_t result = (seq_t)1;      /* the leading 1 in the formula */
    seq_t powk  = (seq_t)1;       /* k^t (t = 0 initially)        */
    seq_t binom  = (seq_t)1;      /* binom(i,0)                    */

    for (idx_t r = 0; r <= i; ++r)
    {
        if (r & 1)                /* odd r => add corresponding term */
        {
            powk *= sk;           /* powk = k^t, where t = (r+1)/2   */
            result += binom * powk;
        }

        if (r == i)
            break;                /* finished all required r values */

        /* update binom to binom(i, r+1) using exact division */
        binom = (binom * (seq_t)(i - r)) / (seq_t)(r + 1);
    }

    return result;
}
