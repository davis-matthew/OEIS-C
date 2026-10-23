/*
 * A134663  Triangle read by rows in which n-th row gives the expansion
 *          coefficients of (1+x+x^4)^n.
 * https://oeis.org/A134663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = coefficient of x^k in (1 + x + x^4)^r where n = 2r² - r +
 *            k, 0 ≤ k ≤ 4r. (The triangle is read row by row.)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   binomial, terms_before128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/terms_before128.h"

static inline idx_t terms_before(idx_t r)
{
    return (idx_t)terms_before128(r);
}

seq_t a(idx_t n)
{
    /* locate the row r with terms_before(r) ≤ n < terms_before(r+1) */
    idx_t low = 0, high = 1;
    while (terms_before128(high) <= (__int128)n) {
        if (high > (idx_t)((UINT64_MAX >> 1)))
            break;                    /* avoid overflow of high */
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if (terms_before128(mid) <= (__int128)n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r   = low;
    idx_t pos = n - terms_before(r);   /* exponent k in the row */

    seq_t result = 0;
    for (idx_t b = 0; b <= pos / 4; ++b) {
        idx_t a = pos - 4 * b;
        if (a > r)          continue;
        if (a + b > r)      continue;
        seq_t part = binomial(r, a);
        part = part * binomial(r - a, b);
        result += part;
    }
    return result;
}
