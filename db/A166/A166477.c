/*
 * A166477  Minimum positive integer solution x of equation
 *          n=x*(x+1)/(t*(t+1)); that is, ratio of product of two consecutive
 *          integers divided by product of two consecutive integers. Here n is
 *          a nonsquare integer (see A000037).
 * https://oeis.org/A166477
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimum positive integer x such that n =
 *            x*(x+1)/(t*(t+1)) for some positive integer t. n is the nth
 *            non‑square integer with offset 2 (a(2) = 3). The implementation
 *            searches over t until the discriminant is a perfect square
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 66, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sqrt_floor.h"   /* provides seq_t sqrt_floor(seq_t) */

static inline seq_t min_x_for_non_square(idx_t nn) {
    /* Search t = 1,2,… until 1 + 4*nn*t*(t+1) is a perfect square. */
    for (idx_t t = 1;; ++t) {
        seq_t rhs = (seq_t)nn * (seq_t)t * (seq_t)(t + 1);     /* nn * t(t+1) */
        seq_t disc = 1 + ((seq_t)4 * rhs);                    /* 1 + 4*rhs   */
        seq_t s = sqrt_floor(disc);                           /* floor sqrt */
        if (s * s == disc) {                                 /* perfect square? */
            return (s - 1) / 2;                              /* x = (s-1)/2  */
        }
    }
}

static inline idx_t nth_non_square(idx_t k) {
    /* Return the k-th non‑square integer (k ≥ 2).  The sequence of
       non‑squares begins 2,3,5,6,7,... So we need the integer m such
       that m - floor(sqrt(m)) = k-1. */
    idx_t low = 2;
    /* Upper bound: k + sqrt(k) + 5 is more than enough for k up to 10⁶. */
    idx_t high = k + (idx_t)sqrt_floor((seq_t)k) + 5;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t cnt = mid - (idx_t)sqrt_floor((seq_t)mid);     /* non‑squares ≤ mid */
        if (cnt >= k - 1)
            high = mid;
        else
            low = mid + 1;
    }
    return low;   /* low is a non‑square integer */
}

seq_t a(idx_t n) {
    /* n is the index into the OEIS sequence (with offset 2).  Translate
       it to the corresponding non‑square integer and compute the
       minimal x. */
    if (n < 2) return 0;   /* undefined for n < 2, but never called. */
    idx_t num = nth_non_square(n);   /* the actual integer in the equation */
    return min_x_for_non_square(num);
}
