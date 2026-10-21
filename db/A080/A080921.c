/*
 * A080921  Duplicate of A053455.
 * https://oeis.org/A080921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2*a(n-1) + 48*a(n-2), a(0)=0, a(1)=1
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 22, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t a0 = 0;
    seq_t a1 = 1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t ai = a1 * 2 + a0 * 48;
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
