/*
 * A060902  Number of ordered factorizations of the identity permutation in
 *          the symmetric group S_n into 2n-2 transpositions such that the
 *          factors generate S_n.
 * https://oeis.org/A060902
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2n-2)! * n^(n-3), with a(2) defined as 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n must be >= 2; values for n < 2 are undefined but return 0 */
    if (n < 2)
        return (seq_t)0;

    /* a(2) is a special case because n^(n-3) would have a negative exponent */
    if (n == 2)
        return (seq_t)1;

    /* factorial of (2n-2) */
    idx_t m = 2 * n - 2;
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= m; ++i)
        fact *= (seq_t)i;

    /* n^(n-3) */
    seq_t power = (seq_t)1;
    for (idx_t i = 0; i < n - 3; ++i)
        power *= (seq_t)n;

    return fact * power;
}
