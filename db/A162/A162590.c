/*
 * A162590  Polynomials with e.g.f. exp(x*t)/csch(t), triangle of coefficients
 *          read by rows.
 * https://oeis.org/A162590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 0 if (r-i) is even, otherwise C(r,i) where r =
 *            floor((sqrt(1+8*n)-1)/2) is the row index and i = n - r*(r+1)/2
 *            is the column index in that row
 * Range:     u8 n <= 48, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = 1;
    /* Exponential search for an upper bound */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) high <<= 1;
    /* Binary search for the correct r */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t prefix = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t i = n - prefix;
    if (((r - i) & 1u) == 0)
        return 0;
    else
        return binomial(r, i);
}
