/*
 * A059332  Determinant of n X n matrix A defined by A[i,j] = (i+j-1)! for 1
 *          <= i,j <= n.
 * https://oeis.org/A059332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{k=1..n} k! * (k-1)! Recurrence: a(0)=1, a(1)=1,
 *            a(n)=a(n-1)*n!*(n-1)! for n≥2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)1;
    }

    seq_t res      = (seq_t)1;
    seq_t factPrev = (seq_t)1;           /* (k-1)! */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t factCurr = factPrev * (seq_t)k;   /* k!    */
        res *= factCurr * factPrev;             /* multiply by k!*(k-1)! */
        factPrev = factCurr;                    /* update for next iteration */
    }

    return res;
}
