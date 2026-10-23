/*
 * A190401  Number of ways to place 6 nonattacking grasshoppers on a toroidal
 *          chessboard of size n x n.
 * https://oeis.org/A190401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            n^2/720*(n^10-15n^8-480n^7+2245n^6+2400n^5+27255n^4-342480n^3+639934n^2+471600n-865080)
 *            + n^2/4*(-1)^n*(8n^4-40n^3+17n^2-272n+1608) for n>8 (Special
 *            values are given for n≤8.)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 69, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* Special cases for n ≤ 8 */
    static const seq_t pre[9] = {
        0ULL,            /* 0 – unused */
        0ULL,            /* a(1) */
        0ULL,            /* a(2) */
        0ULL,            /* a(3) */
        384ULL,          /* a(4) */
        19100ULL,        /* a(5) */
        557808ULL,       /* a(6) */
        5841780ULL,      /* a(7) */
        41338400ULL      /* a(8) */
    };
    if (n < 9) return pre[n];

#ifdef __cplusplus
    /* C++ path – use __int128 for integral types, big‑int for others */
    if constexpr (std::is_integral<seq_t>::value) {
        __int128 N  = (__int128)n;
        __int128 N2 = N * N;
        __int128 N3 = N2 * N;
        __int128 N4 = N3 * N;
        __int128 N5 = N4 * N;
        __int128 N6 = N5 * N;
        __int128 N7 = N6 * N;
        __int128 N8 = N7 * N;
        __int128 N10 = N8 * N2;

        __int128 P = N10
                   - (__int128)15 * N8
                   - (__int128)480 * N7
                   + (__int128)2245 * N6
                   + (__int128)2400 * N5
                   + (__int128)27255 * N4
                   - (__int128)342480 * N3
                   + (__int128)639934 * N2
                   + (__int128)471600 * N
                   - (__int128)865080;

        __int128 term1 = N2 * P / 720;

        __int128 Q = (__int128)8 * N4
                    - (__int128)40 * N3
                    + (__int128)17 * N2
                    - (__int128)272 * N
                    + (__int128)1608;

        __int128 factor = (n & 1) ? -1 : 1;
        __int128 term2 = N2 * Q * factor / 4;

        return (seq_t)(term1 + term2);
    } else {
        /* seq_t is an arbitrary‑precision integer class */
        seq_t N  = (seq_t)n;
        seq_t N2 = N * N;
        seq_t N3 = N2 * N;
        seq_t N4 = N3 * N;
        seq_t N5 = N4 * N;
        seq_t N6 = N5 * N;
        seq_t N7 = N6 * N;
        seq_t N8 = N7 * N;
        seq_t N10 = N8 * N2;

        seq_t P = N10
                - (seq_t)15 * N8
                - (seq_t)480 * N7
                + (seq_t)2245 * N6
                + (seq_t)2400 * N5
                + (seq_t)27255 * N4
                - (seq_t)342480 * N3
                + (seq_t)639934 * N2
                + (seq_t)471600 * N
                - (seq_t)865080;

        seq_t term1 = N2 * P / (seq_t)720;

        seq_t Q = (seq_t)8 * N4
                - (seq_t)40 * N3
                + (seq_t)17 * N2
                - (seq_t)272 * N
                + (seq_t)1608;

        seq_t factor = ((n & 1) ? (seq_t)-1 : (seq_t)1);
        seq_t term2 = N2 * Q * factor / (seq_t)4;

        return term1 + term2;
    }
#else
    /* C path – use __int128 for all intermediate calculations */
    __int128 N  = (__int128)n;
    __int128 N2 = N * N;
    __int128 N3 = N2 * N;
    __int128 N4 = N3 * N;
    __int128 N5 = N4 * N;
    __int128 N6 = N5 * N;
    __int128 N7 = N6 * N;
    __int128 N8 = N7 * N;
    __int128 N10 = N8 * N2;

    __int128 P = N10
               - (__int128)15 * N8
               - (__int128)480 * N7
               + (__int128)2245 * N6
               + (__int128)2400 * N5
               + (__int128)27255 * N4
               - (__int128)342480 * N3
               + (__int128)639934 * N2
               + (__int128)471600 * N
               - (__int128)865080;

    __int128 term1 = N2 * P / 720;

    __int128 Q = (__int128)8 * N4
                - (__int128)40 * N3
                + (__int128)17 * N2
                - (__int128)272 * N
                + (__int128)1608;

    __int128 factor = (n & 1) ? -1 : 1;
    __int128 term2 = N2 * Q * factor / 4;

    return (seq_t)(term1 + term2);
#endif
}
