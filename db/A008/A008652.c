/*
 * A008652  Molien series for group of 3 X 3 upper triangular matrices over
 *          GF( 4 ).
 * https://oeis.org/A008652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (⌊n/16⌋ + 1) * (2·⌊n/16⌋ + ⌊(n mod 16)/4⌋ + 1)
 * Range:     u8 n <= 171, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute m = floor(n/16) and t = floor((n % 16)/4) */
    idx_t m = n / 16;
    idx_t r = n & 15;          /* r = n mod 16 */
    idx_t t = r / 4;           /* t = floor(r/4) */

    /* Compute the result in seq_t to avoid overflow */
    seq_t mm = (seq_t)m;
    seq_t tt = (seq_t)t;
    return (mm + 1) * (2 * mm + tt + 1);
}
