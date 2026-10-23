/*
 * A151983  Numbers congruent to {0, 1} mod 32.
 * https://oeis.org/A151983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32 * floor((n-1)/2) + ((n-1) mod 2)
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This generates the numbers congruent to 0 or 1 modulo 32 in increasing order. */
seq_t a(idx_t n)
{
    seq_t m   = (seq_t)(n - 1) / 2;          /* floor((n-1)/2)   */
    seq_t rem = (seq_t)((n - 1) & 1);       /* (n-1) mod 2      */
    return ((seq_t)32) * m + rem;
}
