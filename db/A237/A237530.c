/*
 * A237530  Number of non-equivalent (mod D_3) ways to choose three points in
 *          an n X n X n triangular grid so that they do not form a 2 X 2 X 2
 *          subtriangle.
 * https://oeis.org/A237530
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 + 3*n^5 - 3*n^4 + 10*n^3 - 48*n^2 + IF(n==1 mod
 *            2)*(27*n^2 - 45*n - 9) + IF(n==1 mod 3)*64)/288
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 103, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for safety on 64‑bit builds. */
    unsigned __int128 N = n;                      /* cast n to 128‑bit */
    unsigned __int128 res = N * N * N * N * N * N; /* n^6 */

    res += (unsigned __int128)3 * N * N * N * N * N; /* + 3*n^5 */
    res -= (unsigned __int128)3 * N * N * N * N;       /* - 3*n^4 */
    res += (unsigned __int128)10 * N * N * N;          /* + 10*n^3 */
    res -= (unsigned __int128)48 * N * N;              /* - 48*n^2 */

    if ((n & 1u) == 1u) { /* n is odd -> IF(n==1 mod 2) */
        res += (unsigned __int128)27 * N * N;          /* + 27*n^2 */
        res -= (unsigned __int128)45 * N;              /* - 45*n */
        res -= (unsigned __int128)9;                   /* - 9 */
    }

    if ((n % 3u) == 1u) { /* IF(n==1 mod 3) */
        res += (unsigned __int128)64;
    }

    res /= (unsigned __int128)288; /* final division */

    return (seq_t)res;
}
