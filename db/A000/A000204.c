/*
 * A000204  Lucas numbers (beginning with 1): L(n) = L(n-1) + L(n-2) with L(1)
 *          = 1, L(2) = 3.
 * https://oeis.org/A000204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  core, easy, nonn, nice
 * Formula:   a(n) = a(n-1) + a(n-2), a(1) = 1, a(2) = 3 (so a(0) = 2)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 2; /* L(0) */
    seq_t y = 1; /* L(1) */
    idx_t i;
    if (n == 0) return x;
    for (i = 1; i < n; i++) {
        seq_t t = x + y;
        x = y;
        y = t;
    }
    return y;
}
