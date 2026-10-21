/*
 * A077519  Duplicate of A078215.
 * https://oeis.org/A077519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(1)=1, a(2)=25, a(3)=3025, a(4)=48400, a(n)=a(n-1)*121 for odd
 *            n ≥ 5, a(n)=a(n-1)*1089 for even n ≥ 5
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 8, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)25;
    if (n == 3) return (seq_t)3025;
    if (n == 4) return (seq_t)48400;
    seq_t res = (seq_t)48400;
    for (idx_t i = 5; i <= n; ++i) {
        /* i is odd -> multiply by 121, even -> 1089 */
        res *= (seq_t)((i & 1) ? 121 : 1089);
    }
    return res;
}
