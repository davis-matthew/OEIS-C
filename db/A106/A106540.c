/*
 * A106540  a(n) = a(n-1) - 2*a(n-2) - 3*a(n-3) - ... - (n-1)*a(1), with a(1)
 *          = a(2) = 1, a(3) = -1.
 * https://oeis.org/A106540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 3*a(n-1) - 5*a(n-2) + 2*a(n-3); Initial values: a(1) = 1,
 *            a(2) = 1, a(3) = -1
 * Range:     i8 n <= 10, i16 n <= 17, i32 n <= 35, i64 n <= 68, i128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)-1;

    seq_t prev3 = (seq_t)1;   /* a(1) */
    seq_t prev2 = (seq_t)1;   /* a(2) */
    seq_t prev1 = (seq_t)-1;  /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t cur = (seq_t)3 * prev1 - (seq_t)5 * prev2 + (seq_t)2 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
