/*
 * A179097  Rectified heptapeton (6-simplex) numbers: the coefficient of
 *          x^(2n-2) in (1+x+x^2+...+x^(n-1))^7.
 * https://oeis.org/A179097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(40 + 26*n + 5*n^2 + 75*n^3 + 75*n^4 + 19*n^5) / 240
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    __uint128_t N = n;
    __uint128_t t = 40
                  + 26 * N
                  + 5 * N * N
                  + 75 * N * N * N
                  + 75 * N * N * N * N
                  + 19 * N * N * N * N * N;
    __uint128_t num = N * t;
    return (seq_t)(num / 240);
}
