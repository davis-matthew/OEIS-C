/*
 * A060182  a(0) = 1, a(1) = 5, a(2) = 13; a(n) = 2*a(n-1) + 2, n > 2.
 * https://oeis.org/A060182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2 for n ≥ 3, with initial values a(0) = 1,
 *            a(1) = 5, a(2) = 13
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)13;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)5;
    seq_t a2 = (seq_t)13;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = a2 * (seq_t)2 + (seq_t)2;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
