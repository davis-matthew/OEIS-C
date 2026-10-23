/*
 * A156353  A symmetrical powers triangle sequence: t(n,m) = (m^(n - m) + (n -
 *          m)^m).
 * https://oeis.org/A156353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = i^(r+1-i) + (r+1-i)^i, where * r =
 *            ceil((sqrt(8*n-7)-1)/2) is the row containing n, * i = n -
 *            r*(r-1)/2 is the column index within that row. * The sequence is
 *            the flattened triangular array of m^(n-m) + (n-m)^m
 * Range:     u8 n <= 23, u16 n <= 58, u32 n <= 157, u64 n <= 412,
 *            u128 n <= 1092, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset starts at 1; undefined for 0 */

    /* Find the row r such that T_{r-1} < n <= T_r, with T_k = k(k+1)/2 */
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid;
    }
    idx_t r = high;                          /* row number */
    idx_t prev_tri = (r - 1) * r / 2;        /* T_{r-1} */
    idx_t i = n - prev_tri;                  /* column index (1‑based) */

    seq_t base1 = (seq_t)i;
    seq_t exp1  = (seq_t)(r + 1 - i);
    seq_t term1 = big_pow(base1, exp1);

    seq_t base2 = (seq_t)(r + 1 - i);
    seq_t exp2  = (seq_t)i;
    seq_t term2 = big_pow(base2, exp2);

    return term1 + term2;
}
