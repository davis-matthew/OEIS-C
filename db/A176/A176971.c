/*
 * A176971  Expansion of (1+x)/(1+x-x^3) in powers of x.
 * https://oeis.org/A176971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + a(n-3); a(0)=1, a(1)=0, a(2)=0
 * Range:     i8 n <= 38, i16 n <= 78, i32 n <= 158, i64 n <= 315,
 *            i128 n <= 631, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 0;
    if (n == 2) return 0;
    seq_t v[3] = { 1, 0, 0 };
    for (idx_t i = 3; i <= n; ++i) {
        v[i % 3] = -v[(i - 1) % 3] + v[(i - 3) % 3];
    }
    return v[n % 3];
}
