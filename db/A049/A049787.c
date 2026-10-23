/*
 * A049787  a(n) = T(n,n-2), array T as in A049783.
 * https://oeis.org/A049787
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 180, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{j=1..n-4} (n-2 mod floor((n-4)/j))
 */
#include <stdint.h>

seq_t a(idx_t n) {
    /* offset is 3; for n < 5 the sum is empty and equals 0 */
    if (n < 5) return 0;

    seq_t sum = 0;
    idx_t n_minus_2 = n - 2;
    idx_t n_minus_4 = n - 4;

    for (idx_t j = 1; j <= n_minus_4; ++j) {
        idx_t k = n_minus_4 / j;           /* floor((n-4)/j) */
        sum += (seq_t)(n_minus_2 % k);    /* (n-2) mod k */
    }
    return sum;
}
