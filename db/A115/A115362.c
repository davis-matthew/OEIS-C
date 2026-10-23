/*
 * A115362  Row sums of ((1,x) + (x,x^2))^(-1)*((1,x)-(x,x^2))^(-1) (using
 *          Riordan array notation).
 * https://oeis.org/A115362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + ⌊v₂(n+1)/2⌋ (1 + the 4‑adic valuation of n+1)
 * Range:     u8 n <= 9896, u16 n <= 9896, u32 n <= 9896, u64 n <= 9896,
 *            u128 n <= 9896, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count trailing zeros of (n+1), i.e. the exponent of 2 dividing it. */
    idx_t m = n + 1;
    idx_t tz = 0;
    while (m != 0 && (m & 1) == 0) {
        m >>= 1;
        tz++;
    }
    /* Return 1 + floor(tz/2). */
    return (seq_t)(1 + tz / 2);
}
