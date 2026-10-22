/*
 * A000179  Ménage numbers: a(0) = 1, a(1) = -1, and for n >= 2, a(n) = number
 *          of permutations s of [0, ..., n-1] such that s(i) != i and s(i) !=
 *          i+1 (mod n) for all i.
 * https://oeis.org/A000179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, nice, easy
 * Formula:   a(n) = ((n^2-2*n)*a(n-1) + n*a(n-2) - 4*(-1)^n)/(n-2) for n >=
 *            3, a(0) = 1, a(1) = -1, a(2) = 0. Rewritten as a(n) = n*a(n-1) +
 *            (n*a(n-2) - 4*(-1)^n)/(n-2), where the division is exact. This
 *            keeps intermediate values no larger than a(n)
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)(-1);
    if (n == 2) return (seq_t)0;

    seq_t prev2 = (seq_t)(-1); /* a(i-2) */
    seq_t prev1 = (seq_t)0;    /* a(i-1) */
    idx_t i;
    for (i = 3; i <= n; i++) {
        seq_t N = (seq_t)i;
        seq_t t = N * prev2;
        if (i % 2 == 0) t -= 4; else t += 4;
        t /= (seq_t)(i - 2);
        seq_t cur = N * prev1 + t;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
