/*
 * A047454  Numbers that are congruent to {1, 2, 3, 4} mod 8.
 * https://oeis.org/A047454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + ((n-1)%4 + 1)
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This sequence consists of all positive integers congruent to 1,2,3,4 modulo 8. */
seq_t a(idx_t n)
{
    /* for n>=1, compute the nth such integer */
    idx_t q = (n - 1) / 4;          /* complete blocks of 4 allowed numbers   */
    idx_t r = (n - 1) % 4;          /* position within the current block      */
    return ((seq_t)8 * q) + ((seq_t)(r + 1));
}
