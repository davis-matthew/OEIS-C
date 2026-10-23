/*
 * A180667  The maximum number of points obtainable in a game of Entanglement
 *          on a board of radius n.
 * https://oeis.org/A180667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (225/2)*n^4 + 45*n^3 - 135*n^2 - (51/2)*n + 44 Offset: 1
 *            (so a(1) is the first term listed)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 78, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* behaviour below offset is irrelevant */

    /* use a signed 128‑bit intermediate to avoid unsigned wrap‑around */
    __int128 N  = (unsigned long long)n;
    __int128 n2 = N * N;
    __int128 n3 = n2 * N;
    __int128 n4 = n2 * n2;

    __int128 num = (__int128)225 * n4
                 + (__int128)90  * n3
                 - (__int128)270 * n2
                 - (__int128)51  * N
                 + (__int128)88;

    return (seq_t)(num / 2);       /* exact integer division */
}
