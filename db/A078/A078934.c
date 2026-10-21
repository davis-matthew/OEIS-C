/*
 * A078934  Smallest semiperimeter of integral rectangle with area n*(n+1)/2.
 * https://oeis.org/A078934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimum over d | T of (d + T/d), where T = n*(n+1)/2 (the
 *            n-th triangular number)
 * Range:     u8 n <= 21, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, if needed */

seq_t a(idx_t n) {
    /* compute the triangular number T = n*(n+1)/2 as seq_t */
    seq_t nn = (seq_t)n;
    seq_t T = nn * (nn + 1) / 2;

    /* initialise best with a large value: T+1 is safe
       because d>=1, other>=T, so d+other <= T+1 */
    seq_t best = T + 1;

    /* iterate over all divisors d up to sqrt(T) */
    for (idx_t d = 1; ; ++d) {
        seq_t dd = (seq_t)d * (seq_t)d;
        if (dd > T) break;
        if (T % d == 0) {
            seq_t other = T / d;
            seq_t sum = d + other;
            if (sum < best) best = sum;
        }
    }
    return best;
}
