/*
 * A061079  Denominators in the series for sin integral Si(x).
 * https://oeis.org/A061079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = (2n-1) * (2n-1)! (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset – the sequence is defined for n ≥ 1. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* m = 2n-1, must be cast to seq_t when used in multiplications. */
    idx_t m = 2 * n - 1;
    seq_t fact = (seq_t)1;

    /* Compute (2n-1)! iteratively.  The loop is safe because m ≥ 1. */
    for (idx_t i = 2; i <= m; ++i) {
        fact *= (seq_t)i;
    }

    /* Return (2n-1) * (2n-1)! */
    return fact * (seq_t)m;
}
