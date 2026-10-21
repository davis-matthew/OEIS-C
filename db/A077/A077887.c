/*
 * A077887  Expansion of (1-x)^(-1)/(1+x^2-2*x^3).
 * https://oeis.org/A077887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n + 1 - a(n-1) - 2*a(n-2); a(0) = 1, a(1) = 1
 * Range:     i8 n <= 16, i16 n <= 32, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial initial values. */
    if (n == 0u)
        return (seq_t)1;
    if (n == 1u)
        return (seq_t)1;

    /* a(n-2) and a(n-1) will be kept in these variables. */
    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)1;   /* a(1) */
    seq_t cur   = 0;          /* placeholder for a(n) */

    /* Iterate from n = 2 up to the requested index. */
    for (idx_t i = 2u; i <= n; ++i) {
        cur = (seq_t)i + 1 - a_nm1 - 2*a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }

    return cur;
}
