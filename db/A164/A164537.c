/*
 * A164537  a(n) = 8*a(n-1) - 14*a(n-2) for n > 1; a(0) = 5, a(1) = 28.
 * https://oeis.org/A164537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 14*a(n-2) ; a(0) = 5, a(1) = 28
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)28;

    seq_t a0 = (seq_t)5;
    seq_t a1 = (seq_t)28;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t an = (seq_t)8 * a1 - (seq_t)14 * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
