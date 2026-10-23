/*
 * A144678  Related to enumeration of quantum states (see reference for
 *          precise definition).
 * https://oeis.org/A144678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n+8} floor(i/4) * floor((n+8-i)/4)
 * Range:     u8 n <= 24, u16 n <= 179, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n + 8;
    seq_t sum = 0;
    for (idx_t i = 1; i <= m; ++i) {
        seq_t term1 = (seq_t)(i / 4);
        seq_t term2 = (seq_t)((m - i) / 4);
        sum += term1 * term2;
    }
    return sum;
}
