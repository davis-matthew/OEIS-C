/*
 * A139581  a(n) = n*(2*n + 19).
 * https://oeis.org/A139581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * (2*n + 19)
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A139581 with external linkage */
/* a(0) = 0, a(1) = 21, a(2) = 46, ... */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * ((sn * (seq_t)2) + (seq_t)19);
}
