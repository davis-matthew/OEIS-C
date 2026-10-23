/*
 * A107908  a(n) = (n+1)*(n+2)^2*(n+3)^2*(n+4)*(3*n+5)/720.
 * https://oeis.org/A107908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)^2*(n+3)^2*(n+4)*(3*n+5)/720
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 49, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 v = 1;
    v *= (unsigned __int128)(n + 1);

    unsigned __int128 t;
    t = (unsigned __int128)(n + 2);
    v *= t * t;

    t = (unsigned __int128)(n + 3);
    v *= t * t;

    v *= (unsigned __int128)(n + 4);

    /* use 128‑bit arithmetic for (3*n+5) to avoid overflow of idx_t */
    v *= (unsigned __int128)((unsigned __int128)n * 3 + 5);

    v /= 720;

    return (seq_t)v;
}
