/*
 * A093117  a(n) = 8*a(n-1) + 21*a(n-2), with a(1)=1, a(2)=15.
 * https://oeis.org/A093117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + 21*a(n-2), with a(1) = 1, a(2) = 15
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0 by convention. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)15;

    /* Initial two terms */
    seq_t a_nm2 = (seq_t)1;   /* a(1)   */
    seq_t a_nm1 = (seq_t)15;  /* a(2)   */

    /* Iteratively compute up to a(n) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur = (seq_t)8 * a_nm1 + (seq_t)21 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }

    return a_nm1;
}
