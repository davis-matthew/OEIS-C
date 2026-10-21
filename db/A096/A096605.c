/*
 * A096605  a(n) = floor((floor(n/2) + a(floor(n/2)))/2), a(1) = 1.
 * https://oeis.org/A096605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((floor(n/2) + a(floor(n/2))) / 2), a(1) = 1
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)          /* base case; a(1) = 1, a(0) never occurs in the official sequence */
        return 1;

    idx_t m = n >> 1;            /* floor(n/2) */
    seq_t am = a(m);             /* recursive call */
    return ((seq_t)m + am) / 2;  /* integer division rounds down automatically */
}
