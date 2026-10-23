/*
 * A101497  Expansion of (1-x^2)/(1-2x+2x^3+x^4).
 * https://oeis.org/A101497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) - a(n-4) for n >= 4 a(0) = 1, a(1) =
 *            2, a(2) = 3, a(3) = 4
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* return known base terms immediately */
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 3;
    if (n == 3) return 4;

    seq_t r[5];
    r[0] = 1;
    r[1] = 2;
    r[2] = 3;
    r[3] = 4;

    for (idx_t i = 4; i <= n; ++i) {
        idx_t idx  = i % 5;
        idx_t i1   = (i-1) % 5;
        idx_t i3   = (i-3) % 5;
        idx_t i4   = (i-4) % 5;
        r[idx] = 2 * r[i1] - 2 * r[i3] - r[i4];
    }
    return r[n % 5];
}
