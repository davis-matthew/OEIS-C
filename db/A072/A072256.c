/*
 * A072256  a(n) = 10*a(n-1) - a(n-2) for n > 1, a(0) = a(1) = 1.
 * https://oeis.org/A072256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - a(n-2) for n > 1, a(0) = a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return 1;

    seq_t a0 = 1;   /* a(n-2) */
    seq_t a1 = 1;   /* a(n-1) */
    seq_t a2;       /* a(n)   */

    for (idx_t i = 2; i <= n; ++i) {
        a2 = 10 * a1 - a0;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
