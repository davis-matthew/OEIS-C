/*
 * A008459  Square the entries of Pascal's triangle.
 * https://oeis.org/A008459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = C(r, k)^2 where r = floor((sqrt(8n + 1) - 1) / 2) and k =
 *            n - r(r + 1)/2. Here C(r, k) is the binomial coefficient
 * Range:     u8 n <= 23, u16 n <= 69, u32 n <= 197, u64 n <= 646,
 *            u128 n <= 1274, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find minimal r with (r+1)(r+2)/2 > n */
    idx_t low  = 0;
    idx_t high = 1;

    /* Expand upper bound until it satisfies the condition */
    while (1) {
        unsigned __int128 t = ((unsigned __int128)(high + 1) * (high + 2)) / 2;
        if (t > n)
            break;
        if (high > (UINT64_MAX >> 1)) {    /* avoid overflow when shifting */
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }

    /* Binary search for the smallest r such that T(r) > n */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 t = ((unsigned __int128)(mid + 1) * (mid + 2)) / 2;
        if (t > n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }

    idx_t r = low;   /* row of the n-th element */

    /* Start index of this row */
    unsigned __int128 start128 = ((unsigned __int128)r * (r + 1)) / 2;
    idx_t start = (idx_t)start128;   /* guaranteed to fit */

    idx_t k = n - start;             /* column index within the row */

    seq_t val = binomial(r, k);
    return val * val;                /* square of the binomial coefficient */
}
