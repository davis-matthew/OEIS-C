/*
 * A215614  Numbers k that are not multiples of 10 and such that sum of digits
 *          of k^2 is 7.
 * https://oeis.org/A215614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th positive integer k that is not a multiple of 10
   and whose square has a digit sum equal to 7.  The search is performed by
   testing successive integers, computing k² using an unsigned 64‑bit integer,
   calculating the digit sum of that square, and counting matches until the
   required index is reached.  For builds with small seq_t types only the
   terms that fit into seq_t are guaranteed to be produced correctly.  */
#include <stdint.h>
#include <stdbool.h>

/* local helper – digit sum of an unsigned 64‑bit integer */
static inline uint64_t digit_sum_u64(uint64_t v) {
    uint64_t sum = 0;
    while (v > 0) {
        sum += v % 10;
        v /= 10;
    }
    return sum;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    for (idx_t k = 1;; ++k) {
        if (k % 10 == 0)          /* skip multiples of 10 */
            continue;

        /* compute square in 64‑bit to avoid overflow in seq_t */
        uint64_t sq = (uint64_t)k * (uint64_t)k;
        if (digit_sum_u64(sq) == 7) {   /* square's digit sum is 7 */
            ++count;
            if (count == n)
                return (seq_t)k;       /* cast to seq_t; results that exceed
                                           the capacity of seq_t are beyond
                                           the required range for that build */
        }
    }
}
