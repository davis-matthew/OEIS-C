/*
 * A107767  a(n) = (1 + 3^n - 2*3^(n/2))/4 if n is even, (1 + 3^n -
 *          4*3^((n-1)/2))/4 if n odd.
 * https://oeis.org/A107767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + 3^n - 2*3^(n/2)) / 4 if n is even a(n) = (1 + 3^n -
 *            4*3^((n-1)/2)) / 4 if n is odd Offset: 1 (i.e. a(1) is the first
 *            term) This implementation uses fast exponentiation for 3^k
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 934 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A107767: a(n) = (1 + 3^n - 2 3^(n/2))/4 for even n, (1 + 3^n - 4 3^((n-1)/2))/4
 * for odd n; by its recurrence a(n) = 4a(n-1) - 12a(n-3) + 9a(n-4) from
 * 0, 1, 4, 16, ring operations only, so exact modulo the width (the original
 * divided by 4 after 3^n had wrapped). */

seq_t a(idx_t n)
{
    static const unsigned char first[4] = {0, 1, 4, 16};
    if (n < 1) return (seq_t)0;
    if (n <= 4) return (seq_t)first[n - 1];
    seq_t w = 0, x = 1, y = 4, z = 16;        /* a(i-3), a(i-2), a(i-1), a(i) */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t t = (seq_t)4 * z - (seq_t)12 * x + (seq_t)9 * w;
        w = x;
        x = y;
        y = z;
        z = t;
    }
    return z;
}
