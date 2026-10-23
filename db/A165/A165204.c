/*
 * A165204  Hankel transform of A165203.
 * https://oeis.org/A165204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 3*a(n-1) - 8*a(n-2) + 3*a(n-3) - a(n-4) for n > 3, with
 *            a(0)=1, a(1)=-5, a(2)=-21, a(3)=-21
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 21, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-5;
    if (n == 2) return (seq_t)-21;
    if (n == 3) return (seq_t)-21;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)-5;
    seq_t a2 = (seq_t)-21;
    seq_t a3 = (seq_t)-21;
    seq_t ai = 0;

    for (idx_t i = 4; i <= n; ++i) {
        ai = (seq_t)3 * a3 - (seq_t)8 * a2 + (seq_t)3 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return ai;
}
