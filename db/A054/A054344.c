/*
 * A054344  Number of ways of covering a 2n X 2n lattice with 2n^2 dominoes of
 *          which exactly 6 are horizontal (or vertical) dominoes.
 * https://oeis.org/A054344
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/720)*n*(n+1)*(120*n^7-300*n^6-70*n^5+363*n^4+416*n^3
 *            -231*n^2-394*n-264)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 14, u64 n <= 168, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 128‑bit arithmetic for safety when seq_t is 64‑bit or smaller */
    __int128 N  = (idx_t)n;
    __int128 poly = (__int128)120;            /* 120 */
    poly = poly * N - (__int128)300;          /* 120x - 300 */
    poly = poly * N - (__int128)70;           /* (120x-300)x - 70 */
    poly = poly * N + (__int128)363;          /* +363 */
    poly = poly * N + (__int128)416;          /* +416 */
    poly = poly * N - (__int128)231;          /* -231 */
    poly = poly * N - (__int128)394;          /* -394 */
    poly = poly * N - (__int128)264;          /* -264 */

    __int128 numerator = N * (N + 1) * poly;
    __int128 result   = numerator / 720;      /* exact division */

    return (seq_t)result;
}
