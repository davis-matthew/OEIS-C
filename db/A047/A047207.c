/*
 * A047207  Numbers that are congruent to {0, 1, 3, 4} mod 5.
 * https://oeis.org/A047207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((5*n-3)/4)
 * Range:     u8 n <= 205, u16 n <= 8000, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is defined; for n < 1 we return 0. */
    if (n < 1) return 0;
    return ((((seq_t)5 * (seq_t)n) - (seq_t)3) / (seq_t)4);
}
