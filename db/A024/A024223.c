/*
 * A024223  [ (4th elementary symmetric function of S(n))/(2nd elementary
 *          symmetric function of S(n)) ], where S(n) = {first n+3 positive
 *          integers congruent to 1 mod 3}.
 * https://oeis.org/A024223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( e4(S(n)) / e2(S(n)) ) where S(n) = { first n+3
 *            positive integers congruent to 1 (mod 3) } e2 = 2nd elementary
 *            symmetric function of S(n) e4 = 4th elementary symmetric
 *            function of S(n)
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t N = n + 3;                /* number of terms in S(n)          */
    __int128 e0 = 1;                /* e0 = 1 (not used further)        */
    __int128 e1 = 0;                /* sum of the elements              */
    __int128 e2 = 0;                /* sum of pairwise products         */
    __int128 e3 = 0;                /* sum of triple products           */
    __int128 e4 = 0;                /* sum of quadruple products        */

    for (idx_t k = 0; k < N; ++k)
    {
        __int128 x = (__int128)k * 3 + 1;  /* current element 3k+1          */

        e4 += x * e3;
        e3 += x * e2;
        e2 += x * e1;
        e1 += x;
    }

    seq_t result = (seq_t)(e4 / e2);  /* floor division, exact for all terms that fit */
    return result;
}
