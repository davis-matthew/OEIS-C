/*
 * A077266  Triangle of number of zeros when n is written in base k (2 <= k <=
 *          n).
 * https://oeis.org/A077266
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn, tabl
 * Formula:   a(n) is the nth element in the flattened triangle where T(m,k) =
 *            number of zero digits in the base‑k representation of the
 *            integer m (2 <= k <= m). The triangle is enumerated row wise
 *            starting with n = 2 (offset 2). This function converts the
 *            sequence index n into the corresponding (row, column)...
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n) {
    if (n < 2) return 0;          /* indices below the offset are irrelevant */

    /* zero‑based position in the flattened triangle */
    idx_t t = n - 2;

    /* solve r*(r-1)/2 > t  for the row number r (r >= 2) */
    long double val = sqrt((long double)1 + (long double)8 * (long double)t);
    uint64_t d = (uint64_t)val;   /* floor of the square‑root */
    idx_t r = (1 + d) / 2;        /* initial estimate */

    /* adjust r if necessary; use 128‑bit to avoid overflow */
    while ( ( (__int128)r * (r - 1) / 2 ) <= t ) ++r;

    /* previous total number of elements before this row */
    idx_t prev_total = ( (__int128)(r - 1) * (r - 2) / 2 );
    /* offset within the row (0‑based) */
    idx_t offset = t - prev_total;
    /* column index k: values in the row run from k = 2 to k = r */
    idx_t k = 2 + offset;

    /* Count zeros in the base‑k representation of the row number r */
    idx_t tmp = r;
    seq_t zeros = 0;
    while (tmp > 0) {
        if ((tmp % k) == 0) ++zeros;
        tmp /= k;
    }
    return zeros;
}
