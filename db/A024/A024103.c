/*
 * A024103  a(n) = 9^n - n^2.
 * https://oeis.org/A024103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n - n^2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow9 = 1;
    seq_t nine = (seq_t)9;
    for (idx_t i = 0; i < n; ++i) {
        pow9 *= nine;
    }

    seq_t n_sq = (seq_t)n * (seq_t)n;
    return pow9 - n_sq;
}
