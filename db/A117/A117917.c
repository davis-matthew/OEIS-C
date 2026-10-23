/*
 * A117917  a(n) = 3*a(n-1) + a(n-2) + n.
 * https://oeis.org/A117917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) + n with a(0) = 1, a(1) = 4
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t a0 = (seq_t)1;   /* a(n-2)   for the current iteration   */
    seq_t a1 = (seq_t)4;   /* a(n-1)   for the current iteration   */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t an = (seq_t)3 * a1 + a0 + (seq_t)i; /* a(n) = 3*a(n-1)+a(n-2)+n */
        a0 = a1;
        a1 = an;
    }
    return a1;
}
