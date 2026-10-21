/*
 * A061477  First (leftmost) digit - second digit + third digit - fourth digit
 *          .... = 8.
 * https://oeis.org/A061477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer whose alternating sum of
 *            decimal digits (leftmost digit added, next subtracted, next
 *            added, …) equals 8
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alt_sum_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alt_sum_ull.h"

/* primary sequence routine */
seq_t a(idx_t n)
{
    unsigned long long cand = 1;   /* iterate over natural numbers */
    idx_t found = 0;

    for (;;) {
        if (alt_sum_ull(cand) == 8) {
            ++found;
            if (found == n) {
                return (seq_t)cand;
            }
        }
        ++cand;   /* candidate fits in 64 bits for the required n ranges */
    }
}
