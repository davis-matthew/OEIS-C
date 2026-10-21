/*
 * A084264  Binomial transform of A084263.
 * https://oeis.org/A084264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n-3) * (n^2 + 3*n + 4) for n > 0; a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;

    /* Compute the polynomial part: (n^2 + 3*n + 4) */
    seq_t poly = (seq_t)n * n + (seq_t)3 * n + (seq_t)4;

    if (n >= 3u)
        return poly << (n - 3u);          /* multiply by 2^(n-3) */
    else
        return poly >> (3u - n);          /* divide by 2^(3-n), exact */
}
