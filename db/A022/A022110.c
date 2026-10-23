/*
 * A022110  Fibonacci sequence beginning 1, 20.
 * https://oeis.org/A022110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 1, a(1) = 20
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)20;

    seq_t a0 = (seq_t)1;   /* a(n-2) */
    seq_t a1 = (seq_t)20;  /* a(n-1) */
    seq_t a;               /* current term */

    for (idx_t i = 2; i <= n; ++i) {
        a = a0 + a1;   /* a(n) = a(n-2) + a(n-1) */
        a0 = a1;       /* shift window forward */
        a1 = a;
    }

    return a1;   /* a(n) */
}
