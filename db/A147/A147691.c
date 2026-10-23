/*
 * A147691  G.f.: x*(1+x+x^2)*(1+6*x+8*x^2+4*x^3-x^4)/((1+x)^2*(1-x)^4).
 * https://oeis.org/A147691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (3*n*(6*n^2-12*n+13)+(n-2)*(-1)^n-6)/8   for n>1
 *   a(0) = 0, a(1) = 1
 * This implements the closed‑form of OEIS A147691.
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Use signed 128‑bit integer for safe intermediate arithmetic. */
    __int128 N = ( __int128 ) n;

    /* Compute 3*n*(6*n^2-12*n+13) */
    __int128 term1 = 3 * N * ( 6 * N * N - 12 * N + 13 );

    /* Compute (n-2)*(-1)^n */
    __int128 term2 = (N - 2) * ( (n & 1) ? -1 : 1 );

    /* Final value: (term1 + term2 - 6) / 8 */
    __int128 result = ( term1 + term2 - 6 ) / 8;

    return (seq_t) result;
}
