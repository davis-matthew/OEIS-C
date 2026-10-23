/*
 * A176849  Triangle read by rows which contains the (6n)-th row of the Pascal
 *          triangle in row n.
 * https://oeis.org/A176849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = binomial(6*r, k) where * r = the largest integer such
 *            that offset(r) = 3*r^2 - 2*r <= n, * k = n - offset(r).
 *            (offset(r) is the starting index of row r.)
 * Range:     u8 n <= 10, u16 n <= 26, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Binary search for r */
    idx_t lo = 0, hi = n + 1;          /* hi is exclusive */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 off = (__int128)mid * (mid - 1) * 3 + mid; /* offset(mid) */
        if (off <= (__int128)n)
            lo = mid;
        else
            hi = mid;
    }
    idx_t r = lo;

    /* Compute offset(r) and column index k */
    __int128 off = (__int128)r * (r - 1) * 3 + r;
    idx_t offset = (idx_t)off;        /* safe: off <= n < 2^64 */
    idx_t k = n - offset;

    /* Return the binomial coefficient */
    idx_t N = 6 * r;
    return binomial(N, k);
}
