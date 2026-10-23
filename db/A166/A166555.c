/*
 * A166555  Triangle read by rows, T(n, k) = 2^k * A047999(n, k).
 * https://oeis.org/A166555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^k if binomial(r,k) mod 2 = 1, otherwise 0, where r is
 *            the unique non‑negative integer with r*(r+1)/2 ≤ n <
 *            (r+1)*(r+2)/2 and k = n - r*(r+1)/2
 * Range:     u8 n <= 43, u16 n <= 151, u32 n <= 559, u64 n <= 2143,
 *            u128 n <= 5150, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* find the row number r */
    idx_t r = 0;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* offset of the first element in row r */
    idx_t offset = (idx_t)((unsigned __int128)r * (r + 1) / 2);

    /* column index within the row */
    idx_t k = n - offset;

    /* binomial(r,k) is odd iff (k & (r - k)) == 0 */
    if ((k & (r - k)) == 0)
        return pow2(k);  /* 2^k */
    else
        return 0;
}
