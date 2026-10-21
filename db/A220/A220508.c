/*
 * A220508  T(n,k) = n^2 + k if k <= n, otherwise T(n,k) = k*(k + 2) - n;
 *          square array T(n,k) read by ascending antidiagonals (n >= 0, k >=
 *          0).
 * https://oeis.org/A220508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) with i = d - t, j = t d = ⌊ (√(8n + 1) - 1) / 2 ⌋
 *            (largest d with d(d+1)/2 ≤ n) t = n - d(d+1)/2 T(i,j) = (j ≤ i)
 *            ? (i*i + j) : (j*(j+2) - i) (Computes the OEIS sequence A220508
 *            – read by ascending antidiagonals)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t low = 0;
    idx_t high = n;
    idx_t d = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n) {
            d = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* offset within diagonal d */
    idx_t tri_d = d * (d + 1) / 2;
    idx_t t = n - tri_d;            /* 0 <= t <= d */

    /* indices i, j */
    idx_t i = d - t;
    idx_t j = t;

    /* Compute T(i,j) in seq_t */
    seq_t ii = (seq_t)i;
    seq_t jj = (seq_t)j;
    seq_t result;

    if (jj <= ii) {
        result = ii * ii + jj;         /* i^2 + j */
    } else {
        result = jj * (jj + 2) - ii;    /* j*(j+2) - i */
    }

    return result;
}
