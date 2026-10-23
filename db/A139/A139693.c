/*
 * A139693  a(n) is the smallest positive integer m where, for k divides m,
 *          minimum(|k - m/k|) = n.
 * https://oeis.org/A139693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1269, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A139693.
   a(n) = the smallest positive integer m such that
          the minimal value of |k - m/k| over all divisors k of m
          equals n.  (Equivalently, the smallest m with A056737(m)=n.) */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

static inline seq_t divisor_diff_min(seq_t m) {
    seq_t min_diff = m;                 /* maximum possible difference is m-1 */
    seq_t sqrt_m = floor_sqrt(m);       /* floor(sqrt(m))  */

    for (seq_t f = 1; f <= sqrt_m; ++f) {
        if (m % f == 0) {
            seq_t g = m / f;            /* complementary divisor */
            seq_t diff = (g >= f) ? (g - f) : (f - g);
            if (diff < min_diff) {
                min_diff = diff;
                if (min_diff == 0)     /* perfect square, stop early */
                    break;
            }
        }
    }
    return min_diff;
}

seq_t a(idx_t n) {
    /* Special case n = 0: m = 1 has divisor difference 0. */
    if (n == 0) return 1;

    /* For each possible divisor d (the smaller factor), test
       m = d * (d + n).  The pair (d, d+n) gives a difference n;
       we must verify that this is indeed the minimal difference. */
    for (idx_t d = 1; ; ++d) {
        seq_t d_seq = (seq_t)d;
        seq_t m = d_seq * (d_seq + (seq_t)n);

        if (divisor_diff_min(m) == n)
            return m;
    }
}
