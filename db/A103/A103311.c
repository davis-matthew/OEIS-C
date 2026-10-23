/*
 * A103311  A transform of the Fibonacci numbers.
 * https://oeis.org/A103311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, changed
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 2*a(n-3) - a(n-4) with initial
 *            values a(0)=0, a(1)=1, a(2)=1, a(3)=0
 * Range:     i8 n <= 13, i16 n <= 24, i32 n <= 48, i64 n <= 93, i128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 1;
    if (n == 3) return 0;

    seq_t a0 = 0;
    seq_t a1 = 1;
    seq_t a2 = 1;
    seq_t a3 = 0;
    seq_t res = 0;

    for (idx_t i = 4; i <= n; ++i) {
        res = 3 * a3 - 4 * a2 + 2 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = res;
    }
    return res;
}
