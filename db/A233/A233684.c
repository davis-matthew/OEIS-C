/*
 * A233684  Number of (n+1) X (1+1) 0..3 arrays with every 2 X 2 subblock
 *          having the sum of the squares of the edge differences equal to 10,
 *          and no two adjacent values equal.
 * https://oeis.org/A233684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 5*a(n-2) - 2*a(n-3); Initial values: a(1)=32,
 *            a(2)=108, a(3)=356
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; 0 is defined as 0 */
    if (n == 1) return (seq_t)32;
    if (n == 2) return (seq_t)108;
    if (n == 3) return (seq_t)356;

    seq_t a_nm3 = (seq_t)32;         /* a(n-3) */
    seq_t a_nm2 = (seq_t)108;        /* a(n-2) */
    seq_t a_nm1 = (seq_t)356;        /* a(n-1) */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        cur = (seq_t)2 * a_nm1 + (seq_t)5 * a_nm2 - (seq_t)2 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return a_nm1;
}
