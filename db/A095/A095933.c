/*
 * A095933  Number of walks of length 2n+1 between two nodes at distance 5 in
 *          the cycle graph C_10.
 * https://oeis.org/A095933
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 13*a(n-2) + 4*a(n-3) Initial values: a(2)=2,
 *            a(3)=14, a(4)=72 Offset is 2. For n < 2 the return value is
 *            unspecified (here 0)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)2;
    if (n == 3)
        return (seq_t)14;
    if (n == 4)
        return (seq_t)72;

    seq_t a_nm3 = (seq_t)2;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)14;  /* a(n-2) */
    seq_t a_nm1 = (seq_t)72;  /* a(n-1) */
    seq_t next;

    for (idx_t i = 5; i <= n; ++i) {
        next = (seq_t)7 * a_nm1 - (seq_t)13 * a_nm2 + (seq_t)4 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }
    return a_nm1;
}
