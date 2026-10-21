/*
 * A228186  a(n) is the smallest natural number k such that (k+n+1)!*(k-n-2)!
 *          < 2*k!*(k-1)!.
 * https://oeis.org/A228186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest k such that (k+n+1)! * (k-n-2)! < 2 * k! *
 *            (k-1)!
 * Range:     u8 n <= 11, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   ratio_lt_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include "../utils/ratio_lt_two.h"

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Minimal valid k is n+2 (k must be > n+1). */
    idx_t low = n + 2;
    idx_t high = low;

    /* Find an upper bound where the inequality holds. */
    while (!ratio_lt_two(n, high))
        high <<= 1;        /* high = high * 2; speeds up search */

    /* Binary search for the smallest k satisfying the inequality. */
    idx_t ans = high;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (ratio_lt_two(n, mid)) {
            ans = mid;
            if (mid == 0) break;  /* avoid underflow when mid == 0 */
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return (seq_t)ans;
}
