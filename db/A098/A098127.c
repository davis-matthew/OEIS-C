/*
 * A098127  Fibonacci sequence with a(1) = 7 and a(2) = 26.
 * https://oeis.org/A098127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2), a(1) = 7, a(2) = 26
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)7;
    if (n == 2)
        return (seq_t)26;

    seq_t a1 = (seq_t)7;
    seq_t a2 = (seq_t)26;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t temp = a1 + a2;
        a1 = a2;
        a2 = temp;
    }
    return a2;
}
