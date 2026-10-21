/*
 * A065033  1 appears three times, other numbers twice.
 * https://oeis.org/A065033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)/2) + 0^n
 * Range:     u8 n <= 338, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;           /* 0^0 = 1, floor((0+1)/2)=0 */
    /* for n > 0 : 0^n = 0 */
    return ((seq_t)n + (seq_t)1) / (seq_t)2;
}
