/*
 * A101382  a(n) = n*(n+1)*(2*n^3 - n^2 + 2)^2/6.
 * https://oeis.org/A101382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(2*n^3 - n^2 + 2)^2/6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 269, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 N     = n;
    unsigned __int128 term1 = N * (N + 1);               /* n*(n+1)           */
    unsigned __int128 n_sq  = N * N;                    /* n^2                */
    unsigned __int128 n_cu  = n_sq * N;                 /* n^3                */
    unsigned __int128 t2    = (unsigned __int128)2 * n_cu - n_sq + (unsigned __int128)2;
    unsigned __int128 t2_sq = t2 * t2;                  /* (2*n^3 - n^2 + 2)^2*/
    unsigned __int128 res128 = term1 * t2_sq / 6;
    return (seq_t)res128;
}
