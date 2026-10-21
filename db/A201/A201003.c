/*
 * A201003  Triangular numbers, T(m), that are four-fifths of another
 *          triangular number: T(m) such that 5*T(m) = 4*T(k) for some k.
 * https://oeis.org/A201003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 36, a(n) = 322*a(n-1) - a(n-2) + 36 for n > 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 36;

    seq_t a0 = 0;          /* a(n-2) */
    seq_t a1 = 36;         /* a(n-1) */
    seq_t temp;

    for (idx_t i = 2; i <= n; ++i) {
        temp = 322 * a1 - a0 + 36;
        a0 = a1;
        a1 = temp;
    }

    return a1;
}
