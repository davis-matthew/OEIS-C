/*
 * A035927  One less than number of n-multisets chosen from a 10-set.
 * https://oeis.org/A035927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+9,9) - 1
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= 9; ++i) {
        res = res * (seq_t)(n + i);
        res = res / (seq_t)i;
    }
    return res - (seq_t)1;
}
