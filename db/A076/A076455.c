/*
 * A076455  Sum of numbers that can be written as t*n + u*(n+1) for
 *          nonnegative integers t,u in exactly two ways.
 * https://oeis.org/A076455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(4*n*n + 2*n - 1)/2
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 214, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 64‑bit index – promote to 128‑bit for safe intermediate arithmetic */
    unsigned __int128 N   = (unsigned __int128)n;
    unsigned __int128 p1  = N * (N + 1) / 2;              /* n(n+1)/2   */
    unsigned __int128 p2  = 4 * N * N + 2 * N - 1;         /* 4n² + 2n – 1 */
    unsigned __int128 res = p1 * p2;                       /* full product */

    return (seq_t)res;
}
