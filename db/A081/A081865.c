/*
 * A081865  a(n) = sigma_7(2n-1).
 * https://oeis.org/A081865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma_7(2n-1) (sum of 7th powers of all divisors of 2n-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 12, u64 n <= 283,
 *            u128 n <= 10000, big any n
 * Helpers:   pow7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow7.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* offset is 1; undefined below, return 0 */
    idx_t m = 2 * n - 1;                 /* odd integer */
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            seq_t d1 = (seq_t)i;
            seq_t d2 = (seq_t)(m / i);
            sum += pow7(d1);
            if (d1 != d2) sum += pow7(d2);
        }
    }
    return sum;
}
