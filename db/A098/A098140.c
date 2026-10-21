/*
 * A098140  63-gonal numbers: a(n) = n*(61*n - 59)/2.
 * https://oeis.org/A098140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(61*n - 59)/2
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * (((seq_t)61 * sn) - 59) / 2;
}
