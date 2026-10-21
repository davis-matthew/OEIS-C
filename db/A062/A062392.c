/*
 * A062392  a(n) = n^4 - (n-1)^4 + (n-2)^4 - ... 0^4.
 * https://oeis.org/A062392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^2 + n - 1)/2 (OEIS A062392)
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 303, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use a wider type for intermediate calculations to avoid overflow (if seq_t is 64‑bit) */
    __int128 N = ( __int128 )n;
    __int128 t  = N * ( N + 1 );                    /* N*(N+1) */
    t = t * ( N * N + N - 1 );                     /* N*(N+1)*(N^2+N-1) */
    t /= 2;                                         /* divide by 2 */
    return (seq_t)t;                                /* cast back to seq_t */
}
