/*
 * A153792  12 times pentagonal numbers: a(n) = 6*n*(3*n-1).
 * https://oeis.org/A153792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n*(3*n-1) = 18*n^2 - 6*n
 * Range:     u8 n <= 3, u16 n <= 60, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A153792 (12 times pentagonal numbers) */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * ((nn * 3) - 1) * 6;
}
