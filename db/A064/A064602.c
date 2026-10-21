/*
 * A064602  Partial sums of A001157: Sum_{j=1..n} sigma_2(j).
 * https://oeis.org/A064602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} k^2 * ⌊n/k⌋ (partial sums of σ₂)
 * Range:     u8 n <= 8, u16 n <= 54, u32 n <= 2204, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kv = (seq_t)k;          /* k in seq_t to avoid overflow in multiplication   */
        seq_t t  = (seq_t)(n / k);    /* floor(n/k) fits in seq_t                         */
        total += kv * kv * t;         /* compute k² * floor(n/k) and add to total          */
    }
    return total;
}
