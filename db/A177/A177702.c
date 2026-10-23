/*
 * A177702  Period 3: repeat [1, 1, 2].
 * https://oeis.org/A177702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, nonn, easy
 * Formula:   a(n) = a(n-3) for n > 2, with a(0) = 1, a(1) = 1, a(2) = 2
 *            Period 3: sequence [1, 1, 2] repeating
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* (n % 3) is 0, 1, or 2.  If 2 we return 2, otherwise 1. */
    idx_t r = n % (idx_t)3;          /* modulo 3 on unsigned index */
    return (r == (idx_t)2 ? (seq_t)2 : (seq_t)1);
}
