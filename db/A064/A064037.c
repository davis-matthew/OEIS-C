/*
 * A064037  Number of walks of length 2n on cubic lattice, starting and
 *          finishing at origin and staying in first (nonnegative) octant.
 * https://oeis.org/A064037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*(2*n-1)*(5*n*n+10*n+3)*a(n-1) -
 *            36*(n-1)*(2*n-1)*(2*n-3)*a(n-2)) / ((n+3)*(n+2)*(n+1)) with
 *            a(0)=1, a(1)=3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)3;   /* a(1) */
    seq_t an   = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* Use 128‑bit arithmetic for intermediate products */
        __int128 term1 = (__int128)4 *
                         (2*i-1) *
                         (5*i*i + 10*i + 3) *
                         (unsigned __int128)a_nm1;

        __int128 term2 = (__int128)36 *
                         (i-1) *
                         (2*i-1) *
                         (2*i-3) *
                         (unsigned __int128)a_nm2;

        __int128 num = term1 - term2;
        __int128 den = (__int128)(i+3)*(i+2)*(i+1);

        an = (seq_t)(num / den);   /* exact division */
        a_nm2 = a_nm1;
        a_nm1 = an;
    }
    return an;
}
