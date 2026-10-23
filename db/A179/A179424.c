/*
 * A179424  Number of ways to place 4 nonattacking kings on an n X n toroidal
 *          board.
 * https://oeis.org/A179424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/24 * n^2 * (n^6 - 54 n^4 + 1019 n^2 - 6798) for n >= 5
 *            a(1) = 0, a(2) = 0, a(3) = 0, a(4) = 12
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 24, u64 n <= 380, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Special cases for n <= 4 */
    if (n <= 3) return 0;
    if (n == 4) return 12;

    /* Use 128‑bit arithmetic for the intermediate computation
       to avoid overflow for n up to at least 10000. */
    unsigned __int128 N  = (unsigned __int128)n;
    unsigned __int128 N2 = N * N;          /* n^2    */
    unsigned __int128 N4 = N2 * N2;        /* n^4    */
    unsigned __int128 N6 = N4 * N2;        /* n^6    */

    unsigned __int128 expr = N6
                           - (unsigned __int128)54 * N4
                           + (unsigned __int128)1019 * N2
                           - (unsigned __int128)6798;

    unsigned __int128 prod   = N2 * expr;   /* n^2 * expr */
    unsigned __int128 result = prod / (unsigned __int128)24;

    return (seq_t)result;                  /* cast back to seq_t */
}
