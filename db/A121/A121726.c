/*
 * A121726  Sum sequence A000522 then subtract 0,1,2,3,4,5,...
 * https://oeis.org/A121726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{t=1..n} C(n,t)*(t-1)! - (n-1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t comb = 1;  /* C(n,0) */
    seq_t fact = 1;  /* (t-1)! for t=1 */

    for (idx_t t = 1; t <= n; ++t) {
        /* update comb to C(n,t) */
        comb = (comb * (seq_t)(n - t + 1)) / (seq_t)t;
        /* add term C(n,t)*(t-1)! */
        sum += comb * fact;
        /* prepare (t)! for next iteration (fact holds (t-1)! now) */
        fact *= t;
    }
    /* subtract (n-1) */
    return sum - (seq_t)(n - 1);
}
