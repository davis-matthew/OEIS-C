/*
 * A031878  Maximal number of edges in Hamiltonian path in complete graph on n
 *          nodes.
 * https://oeis.org/A031878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)/2 - (n%2==0 ? (n/2 - 1) : 0)
 * Range:     u8 n <= 16, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t res = n_seq * (n_seq - 1);      /* n*(n-1) */
    if ((n & 1) == 0)                     /* if n is even */
        res -= n_seq - 2;                 /* subtract (n/2 -1)*2 = n-2 */
    res /= 2;                             /* divide by 2 */
    return res;
}
