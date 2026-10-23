/*
 * A040799  Continued fraction for sqrt(828).
 * https://oeis.org/A040799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 28; for n >= 1, a(n) = a(n-6) for n >= 7. The sequence
 *            repeats the block [1, 3, 2, 3, 1, 56] for all indices n ≥ 1
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {1, 3, 2, 3, 1, 56};
    if (n == 0)
        return 28;
    return pattern[(n - 1) % 6];
}
