/*
 * A110657  a(n) = A028242(A028242(n)).
 * https://oeis.org/A110657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = floor(n/4) + (n mod 4) mod 3
 * Range:     u8 n <= 1016, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t q = n_seq / 4;
    seq_t r = n_seq % 4;
    return q + (r % 3);
}
