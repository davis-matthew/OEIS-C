/*
 * A061997  Number of ways to place 4 nonattacking kings on an n X n board.
 * https://oeis.org/A061997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^8 - 54*n^6 + 72*n^5 + 995*n^4 - 2472*n^3 - 5094*n^2 +
 *            21480*n - 17112) / 24 (valid for n >= 3)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 24, u64 n <= 380, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    /* Compute using 128‑bit integer to avoid intermediate overflow. */
    __int128 N   = n;
    __int128 n2  = N * N;
    __int128 n3  = n2 * N;
    __int128 n4  = n3 * N;
    __int128 n5  = n4 * N;
    __int128 n6  = n5 * N;
    __int128 n8  = n6 * n2;        /* n8 = n^8 */

    __int128 num = n8
                   - 54 * n6
                   + 72 * n5
                   + 995 * n4
                   - 2472 * n3
                   - 5094 * n2
                   + 21480 * N
                   - 17112;
    num /= 24;                      /* guaranteed integer */

    return (seq_t)num;
}
