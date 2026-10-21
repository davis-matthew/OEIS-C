/*
 * A064842  Maximal value of Sum_{i=1..n} (p(i) - p(i+1))^2, where p(n+1) =
 *          p(1), as p ranges over all permutations of {1, 2, ..., n}.
 * https://oeis.org/A064842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 - 8*n + 3*(-1)^n + 9)/6
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;          /* n^2 */
    seq_t n3  = n2 * N;         /* n^3 */
    seq_t num = (seq_t)2 * n3;  /* 2*n^3 */

    /* add 12 for even n, 6 for odd n */
    num = num + ((n & 1) ? (seq_t)6 : (seq_t)12);

    /* subtract 8*n */
    num = num - (seq_t)8 * N;

    return num / (seq_t)6;
}
