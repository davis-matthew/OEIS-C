/*
 * A155155  a(n) = 2*(10*3^n - 1).
 * https://oeis.org/A155155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2); a(0) = 18, a(1) = 58
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)18;
    if (n == 1)
        return (seq_t)58;

    seq_t a0 = (seq_t)18;          /* a(0) */
    seq_t a1 = (seq_t)58;          /* a(1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        an = 4 * a1 - 3 * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
