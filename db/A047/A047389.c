/*
 * A047389  Numbers that are congruent to {3, 5} mod 7.
 * https://oeis.org/A047389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*n - 5 - 3*(-1)^n)/4, offset 1
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 14*n - 5 once */
    seq_t p = (seq_t)14 * (seq_t)n - 5;

    /* If n is odd, add 3; if n is even, subtract 3 */
    if (n & 1U)   /* odd */
        return (p + 3) / 4;
    else          /* even */
        return (p - 3) / 4;
}
