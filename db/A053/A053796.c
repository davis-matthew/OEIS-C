/*
 * A053796  a(n) = (n^2+n) modulo 5.
 * https://oeis.org/A053796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + n) modulo 5
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t n_mod5 = n % 5;
    idx_t n1_mod5 = (n_mod5 + 1) % 5;
    seq_t prod = (seq_t)n_mod5 * (seq_t)n1_mod5;
    return prod % 5;
}
