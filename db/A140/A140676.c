/*
 * A140676  a(n) = n*(3*n + 4).
 * https://oeis.org/A140676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3*n + 4)
 * Range:     u8 n <= 8, u16 n <= 147, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t three = (seq_t)3;
    seq_t four  = (seq_t)4;
    return m * (m * three + four);
}
