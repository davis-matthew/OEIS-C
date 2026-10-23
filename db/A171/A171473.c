/*
 * A171473  a(n) = 6*a(n-1) - 8*a(n-2)-3 for n > 1; a(0) = 35, a(1) = 135.
 * https://oeis.org/A171473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 35, a(1) = 135, a(n) = 6*a(n-1) - 8*a(n-2) - 3 for n > 1
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)35;
    if (n == 1) return (seq_t)135;
    seq_t a0 = (seq_t)35;
    seq_t a1 = (seq_t)135;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t a2 = 6 * a1 - 8 * a0 - (seq_t)3;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
