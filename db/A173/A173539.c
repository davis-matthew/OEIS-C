/*
 * A173539  Square array read by antidiagonals: T(n,k)=0 if k is a divisor of
 *          n, otherwise T(n,k)=k.
 * https://oeis.org/A173539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = value read antidiagonally from the square array T(n,k)
 *            where T(n,k) = 0 if k divides n, otherwise T(n,k) = k
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of A173539. */
seq_t a(idx_t n)
{
    /* Find the diagonal s such that the cumulative count up to that diagonal
       is at least n.  The number of terms along diagonal s (sum index)
       equals s-1. */
    idx_t s    = 2;      /* diagonal sum index starts at 2 (for n=1,k=1) */
    idx_t cum  = 0;      /* cumulative count of terms before current diagonal */

    while (1) {
        idx_t add = s - 1;          /* elements on this diagonal */
        if (cum + add >= n) break;  /* we've reached the diagonal containing n */
        cum += add;
        s++;
    }

    /* Position of n within the diagonal (0‑based). */
    idx_t offset = n - cum - 1;

    /* Column index k varies from 1 upwards along the diagonal. */
    idx_t k   = offset + 1;
    /* Row index nIdx is the remaining part of the diagonal sum. */
    idx_t nIdx = s - k;

    /* T(nIdx,k) = 0 if k divides nIdx, else k. */
    if (nIdx % k == 0) {
        return (seq_t)0;
    } else {
        return (seq_t)k;
    }
}
