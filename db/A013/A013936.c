/*
 * A013936  a(n) = Sum_{k=1..n} floor(n/k^2).
 * https://oeis.org/A013936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n/k^2)
 * Range:     u8 n <= 166, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, but guard against 0 */
    seq_t sum = 0;
    for (idx_t k = 1; ; ++k) {
        idx_t kk = k * k;
        if (kk > n) break;
        sum += (seq_t)(n / kk);
    }
    return sum;
}
