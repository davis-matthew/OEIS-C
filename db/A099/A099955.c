/*
 * A099955  Atomic numbers in first column in the Mendeleyev periodic table of
 *          elements.
 * https://oeis.org/A099955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = A018227(n-1) + 1
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    seq_t sum = 0;
    for (idx_t i = 1; i < n; ++i) {
        seq_t k = (seq_t)(i / 2);
        seq_t L = (seq_t)2 * k * k + (seq_t)4 * k + (seq_t)2;
        sum += L;
    }
    return (seq_t)1 + sum;
}
