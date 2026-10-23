/*
 * A132757  a(n) = n*(n+29)/2.
 * https://oeis.org/A132757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+29)/2
 * Range:     u8 n <= 12, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows directly from the OEIS entry for A132757. */

seq_t a(idx_t n)
{
    return ((seq_t)n * ((seq_t)n + 29)) / 2;
}
