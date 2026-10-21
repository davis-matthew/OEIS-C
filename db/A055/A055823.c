/*
 * A055823  a(n) = T(n,n-6), array T as in A055818.
 * https://oeis.org/A055823
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^6 + 15*n^5 - 65*n^4 - 795*n^3 + 1864*n^2 + 6180*n -
 *            7200)/720 a(6) = 1 This holds for all n ≥ 6
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 118, u64 n <= 4863,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset and the trivial base case. */
    if (n == 6) return (seq_t)1;
    if (n < 6) return (seq_t)0;   /* behaviour below offset is undefined, return 0 */

    /* Compute the expression safely using a signed 128‑bit integer. */
    __int128 N = n;

    __int128 num = N*N*N*N*N*N               /* n^6   */
                + (__int128)15 * N*N*N*N*N  /* +15*n^5 */
                - (__int128)65 * N*N*N*N    /* -65*n^4 */
                - (__int128)795 * N*N*N     /* -795*n^3 */
                + (__int128)1864 * N*N      /* +1864*n^2 */
                + (__int128)6180 * N        /* +6180*n */
                - (__int128)7200;           /* -7200    */

    /* Divide by 720.  The division is exact for all integer n ≥ 6. */
    seq_t result = (seq_t)(num / 720);
    return result;
}
