/*
 * A206092  Number of (n+1) X 7 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly two counterclockwise and two clockwise edge
 *          increases.
 * https://oeis.org/A206092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 130 * a(n-3) for n > 9 base values a(1)..a(9) are
 *            provided explicitly below
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 23, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* base values for n = 1 .. 9 */
    static const seq_t base[9] = {
        52812ULL,
        54432ULL,
        86496ULL,
        444480ULL,
        2156544ULL,
        10584000ULL,
        56633472ULL,
        279244800ULL,
        1373929344ULL
    };

    if (n <= 9) {
        /* array index is zero‑based */
        return base[n - 1];
    }

    /* number of times we must apply the recurrence a(n) = 130 * a(n-3) */
    idx_t k    = (n - 10) / 3 + 1;
    idx_t idx  = n - 3 * k;          /* idx will be between 1 and 9 */

    seq_t result = base[idx - 1];
    for (idx_t i = 0; i < k; ++i) {
        result *= 130ULL;            /* repeated multiplication */
    }
    return result;
}
