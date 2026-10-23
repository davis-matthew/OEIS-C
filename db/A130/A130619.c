/*
 * A130619  Let M(n) = {{n, 0, 1}, {1, 0, 0}, {0, 1, 0}}, then a(n) is the
 *          upper-right term of M(n)*M(n-1)*...*M(1).
 * https://oeis.org/A130619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*a(n-1) + a(n-3), a(0)=0, a(1)=1, a(2)=2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0ULL) {
        return (seq_t)0;
    }
    if (n == 1ULL) {
        return (seq_t)1;
    }
    if (n == 2ULL) {
        return (seq_t)2;
    }

    /* Iterative computation using only the last three values */
    seq_t a_nm3 = (seq_t)0;   /* a(0)   */
    seq_t a_nm2 = (seq_t)1;   /* a(1)   */
    seq_t a_nm1 = (seq_t)2;   /* a(2)   */
    seq_t a_n   = 0;

    for (idx_t i = 3ULL; i <= n; ++i) {
        a_n   = (seq_t)i * a_nm1 + a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
