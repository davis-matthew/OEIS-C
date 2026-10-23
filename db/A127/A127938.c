/*
 * A127938  Number of arithmetic progressions of 2 or more nonnegative
 *          integers, strictly increasing with sum n.
 * https://oeis.org/A127938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A049982(n) + A007862(n) (number of arithmetic
 *            progressions of 2+ nonnegative integers with sum n)
 * Range:     u8 n <= 239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t sn = (seq_t)n;

    for (idx_t m = 2; ; ++m) {
        seq_t tri = ((seq_t)m * (m - 1)) / 2;   /* triangular number t_{m-1} = m*(m-1)/2 */
        if (tri > sn) break;

        idx_t max_w = (idx_t)(sn / tri);      /* maximum w such that m*b + w*tri <= n */

        for (idx_t w = 1; w <= max_w; ++w) {
            seq_t rest = sn - tri * (seq_t)w;  /* remaining part to be distributed as m*b */
            if (rest % (seq_t)m == 0) {
                /* b = rest / m will be non‑negative automatically since rest <= sn */
                count += 1;
            }
        }
    }

    return count;
}
