/*
 * A010730  a(n) = (17 - 3*(-1)^n)/2.
 * https://oeis.org/A010730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (17 - 3*(-1)^n)/2
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to a(odd) = 10, a(even) = 7 */
seq_t a(idx_t n)
{
    /* Use the parity of n: even -> 7, odd -> 10 */
    return (n & 1) ? (seq_t)10 : (seq_t)7;
}
