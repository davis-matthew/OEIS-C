/*
 * A080232  Triangle T(n,k) of differences of pairs of consecutive terms of
 *          triangle A071919.
 * https://oeis.org/A080232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   The sequence is read by rows from triangle T(n,k) where T(r,k) =
 *            binomial(r-1,k) - binomial(r-1,k-1) (T(0,0)=1, T(r,0)=1,
 *            T(r,r)=-1). For a given index n, find r such that r(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2, let k = n - r(r+1)/2. Then a(n) = T(r,k)
 * Range:     i8 n <= 47, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* locate row r */
    idx_t low = 0, high = 1;
    while ((__int128)high * (high + 1) / 2 <= n) high <<= 1;
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;

    idx_t row_start = (idx_t)((__int128)r * (r + 1) / 2);
    idx_t k = n - row_start;   /* column within the row */

    if (r == 0) return 1;          /* T(0,0)  */
    if (k == 0) return 1;          /* T(r,0)  */
    if (k == r) return -1;         /* T(r,r)  */

    seq_t bin_k   = binomial(r - 1, k);
    seq_t bin_km1 = binomial(r - 1, k - 1);
    return bin_k - bin_km1;
}
