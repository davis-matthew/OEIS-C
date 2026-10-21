/*
 * A067574  Array T(i,j) read by ascending antidiagonals, where T(i,j) is the
 *          concatenation of i and j (1<=i, 1<=j).
 * https://oeis.org/A067574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   a(n) = concatenation of i and j where i = s - k, j = k, s =
 *            smallest integer with s*(s-1)/2 ≥ n, k = n - (s-1)*(s-2)/2,
 *            T(i,j) = i * 10^{digits(j)} + j
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1 */

    /* Find the antidiagonal index s such that s*(s-1)/2 ≥ n. */
    idx_t low = 2, high = 2;
    while ((__uint128_t)high * (high - 1) / 2 < n) {
        high <<= 1;
        /* high will never overflow for reasonable n */
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t val = (__uint128_t)mid * (mid - 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;

    /* Position k within this antidiagonal (1‑based). */
    __uint128_t prev = (__uint128_t)(s - 1) * (s - 2) / 2;
    idx_t k = (idx_t)(n - prev);

    /* Coordinates */
    idx_t i = s - k;
    idx_t j = k;

    /* Number of decimal digits of j */
    int d = 1;
    idx_t tmp = j;
    while (tmp >= 10) {
        tmp /= 10;
        d++;
    }

    /* Concatenate: i * 10^digits(j) + j */
    return (seq_t)i * pow10((idx_t)d) + (seq_t)j;
}
