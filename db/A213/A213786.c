/*
 * A213786  a(n) = Sum_{1<=i<j<=n} b(i)*b(j), where b(k) = A020985(k).
 * https://oeis.org/A213786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{1<=i<j<=n} b(i)*b(j) with b(k) = A020985(k);
 *            computed as a running sum: a(n) = a(n-1) + b(n)*S(n-1), S(n) =
 *            Sum_{k=1..n} b(k), a(0) = 0
 * Range:     i8 n <= 135, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9769 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int A020985_sign(idx_t n)
{
    int sign = 1;
    while (n > 1) {
        if ((n & 1) && ((n >> 1) & 1))
            sign = -sign;
        n >>= 1;
    }
    return sign;
}

seq_t a(idx_t n)
{
    seq_t total = 0;  /* a(m) */
    seq_t s = 0;      /* S(m) */
    for (idx_t k = 1; k <= n; ++k) {
        int b = A020985_sign(k);
        if (b > 0) {
            total += s;
            s += 1;
        } else {
            total -= s;
            s -= 1;
        }
    }
    return total;
}
