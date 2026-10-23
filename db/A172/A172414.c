/*
 * A172414  Triangle read by rows: Catalan number C(n) repeated 2*n+1 times.
 * https://oeis.org/A172414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy, less
 * Formula:   a(n) = catalan(floor(sqrt(n)))
 * Range:     u8 n <= 35, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

/* integer square‑root, floor(sqrt(x)) for idx_t values.
   Uses only integer arithmetic. */
static idx_t int_sqrt(idx_t x)
{
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {            /* avoid division by zero */
            low = mid + 1;
            continue;
        }
        if (mid <= x / mid) {      /* mid*mid <= x  without overflow */
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return low - 1;                /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    return catalan(int_sqrt(n));
}
