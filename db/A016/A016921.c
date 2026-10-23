/*
 * A016921  a(n) = 6*n + 1.
 * https://oeis.org/A016921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n + 1
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute using seq_t to avoid overflow in intermediate results. */
    seq_t val = ((seq_t)n) * 6 + 1;
    return val;
}
