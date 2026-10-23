/*
 * A108642  a(n) = (72 - 258*n + 601*n^2 - 264*n^3 + 33*n^4)/4.
 * https://oeis.org/A108642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = (72 - 258*n + 601*n^2 - 264*n^3 + 33*n^4)/4
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 126, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;
    seq_t sn2  = sn * sn;
    seq_t sn3  = sn2 * sn;
    seq_t sn4  = sn3 * sn;

    seq_t numerator = (seq_t)33 * sn4
                     - (seq_t)264 * sn3
                     + (seq_t)601 * sn2
                     - (seq_t)258 * sn
                     + (seq_t)72;

    return numerator / (seq_t)4;
}
