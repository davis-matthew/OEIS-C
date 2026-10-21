/*
 * A227906  Coins left after packing heart patterns (fixed orientation) into n
 *          X n coins.
 * https://oeis.org/A227906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n even; a(n) = 2*n-1 if n odd
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* if n is even return n, otherwise return 2*n-1 */
    if ((n & 1) == 0) {
        return (seq_t)n;
    } else {
        return (seq_t)(2 * (seq_t)n - 1);
    }
}
