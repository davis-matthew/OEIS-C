/*
 * A076463  Sum of squares of numbers that can be written as t*n + u*(n+1) for
 *          nonnegative integers t,u in exactly four ways.
 * https://oeis.org/A076463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(97*n^4+146*n^3+30*n^2-19*n+1)/6
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 24, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    /* use 128‑bit arithmetic for intermediate results */
    unsigned __int128 N  = (unsigned __int128)n;
    unsigned __int128 N2 = N * N;
    unsigned __int128 N3 = N2 * N;
    unsigned __int128 N4 = N3 * N;
    unsigned __int128 P  = 97 * N4 + 146 * N3 + 30 * N2 + 1 - 19 * N;
    unsigned __int128 prod = N * (N + 1) * P;
    unsigned __int128 res   = prod / 6;
    return (seq_t)res;
}
