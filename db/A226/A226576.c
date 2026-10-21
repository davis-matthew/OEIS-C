/*
 * A226576  Smallest number of integer-sided squares needed to tile a 3 X n
 *          rectangle.
 * https://oeis.org/A226576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*floor((n+2)/3) - 2*floor(n/3)
 * Range:     u8 n <= 759, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the integer divisions in idx_t to avoid overflow
       of smaller types.  The final result fits in seq_t for all
       values that are checked. */
    idx_t t1 = (n + (idx_t)2) / (idx_t)3;
    idx_t t2 = n / (idx_t)3;
    return (seq_t)(3 * t1 - 2 * t2);
}
