/*
 * A172967  Number of ways to place 5 nonattacking knights on an n X n
 *          cylindrical board.
 * https://oeis.org/A172967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            n*(n^9-90*n^7+120*n^6+3395*n^5-8160*n^4-62130*n^3+204000*n^2+463464*n-1888080)/120,
 *            n>=10
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 15, u64 n <= 136, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small values (n < 10) are hard‑coded */
    if (n < 10) {
        switch (n) {
            case 1: return (seq_t)0;
            case 2: return (seq_t)0;
            case 3: return (seq_t)0;
            case 4: return (seq_t)208;
            case 5: return (seq_t)3210;
            case 6: return (seq_t)58056;
            case 7: return (seq_t)458157;
            case 8: return (seq_t)2524176;
            case 9: return (seq_t)10587591;
        }
    }

    /* Compute with 128‑bit arithmetic to avoid overflow for all n kept in the test set */
    unsigned __int128 N    = (unsigned __int128)n;
    unsigned __int128 N2   = N * N;
    unsigned __int128 N3   = N2 * N;
    unsigned __int128 N4   = N3 * N;
    unsigned __int128 N5   = N4 * N;
    unsigned __int128 N6   = N5 * N;
    unsigned __int128 N7   = N6 * N;
    unsigned __int128 N8   = N7 * N;
    unsigned __int128 N9   = N8 * N;

    /* Polynomial part of the formula */
    unsigned __int128 poly =
            N9
          - (unsigned __int128)90 * N7
          + (unsigned __int128)120 * N6
          + (unsigned __int128)3395 * N5
          - (unsigned __int128)8160 * N4
          - (unsigned __int128)62130 * N3
          + (unsigned __int128)204000 * N2
          + (unsigned __int128)463464 * N
          - (unsigned __int128)1888080;

    /* Compute (poly * N) / 120 by splitting to stay inside 128 bits */
    unsigned __int128 a  = poly / 120;
    unsigned __int128 b  = poly % 120;
    unsigned __int128 result = a * N;
    result += (b * N) / 120;

    return (seq_t)result;
}
