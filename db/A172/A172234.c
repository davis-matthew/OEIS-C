/*
 * A172234  Number of ways to place 7 nonattacking wazirs on a 7 X n board.
 * https://oeis.org/A172234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (117649*n^7-1663893*n^6+10942729*n^5-43685355*n^4+114945646*n^3-199980312*n^2+213228096*n-107390880)/720
 *            (valid for n ≥ 6). For n = 1..5 the values are hard‑coded from
 *            the sequence list
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 13, u64 n <= 274, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 6) {
        static const seq_t small[5] = {0, 2, 1478, 50726, 573797};
        return small[n-1];
    }
    signed __int128 num = 0;
    const signed __int128 coeffs[8] = {
        117649,          // n^7 coefficient
        -1663893,        // n^6 coefficient
        10942729,        // n^5 coefficient
        -43685355,       // n^4 coefficient
        114945646,       // n^3 coefficient
        -199980312,      // n^2 coefficient
        213228096,       // n^1 coefficient
        -107390880       // n^0 coefficient
    };
    idx_t i;
    for (i = 0; i < 8; ++i)
        num = num * n + coeffs[i];
    return (seq_t)(num / 720);
}
