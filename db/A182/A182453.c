/*
 * A182453  a(n) = 3^n - n*(n-1)/2.
 * https://oeis.org/A182453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - n*(n-1)/2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 3^n */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow3 = pow3 * (seq_t)3;

    /* compute n*(n-1)/2, 0 for n<2 */
    seq_t binom = 0;
    if (n >= 2)
        binom = (seq_t)n * (seq_t)(n - 1) / 2;

    return pow3 - binom;
}
