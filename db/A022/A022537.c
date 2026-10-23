/*
 * A022537  Nexus numbers (n+1)^21 - n^21.
 * https://oeis.org/A022537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^21 - n^21
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 7, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n1 = (seq_t)n + 1;
    seq_t n_pow = 1;
    seq_t n1_pow = 1;
    for (idx_t i = 0; i < 21; ++i) {
        n_pow *= (seq_t)n;
        n1_pow *= n1;
    }
    return n1_pow - n_pow;
}
