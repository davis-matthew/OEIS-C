/*
 * A047622  Numbers that are congruent to {0, 3, 5} mod 8.
 * https://oeis.org/A047622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((8*n-7)/3)
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is non‑negative; for n>=1 the formula is valid. */
    seq_t t = (seq_t)8 * n - (seq_t)7;
    return t / (seq_t)3;
}
