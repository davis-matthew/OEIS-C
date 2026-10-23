/*
 * A160941  a(n)= n - digital sum(n-1).
 * https://oeis.org/A160941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n - digital_sum(n-1)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 (offset 1) */
    uint64_t m = (uint64_t)n - 1;          /* value of n-1 fits in 64 bits */
    uint64_t sum = 0;
    while (m > 0) {                      /* compute decimal digit sum of n-1 */
        sum += m % 10;
        m /= 10;
    }
    /* compute n - sum in seq_t */
    return (seq_t)n - (seq_t)sum;
}
