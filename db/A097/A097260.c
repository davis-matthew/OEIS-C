/*
 * A097260  Numbers whose set of base 14 digits is {0,D}, where D base 14 = 13
 *          base 10.
 * https://oeis.org/A097260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 13 * \sum_{i \ge 0} (bit_i(n-1) * 14^i)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (bit_i(x) = 0 or 1, the i-th bit of x) */
#include <stddef.h>

seq_t a(idx_t n) {
    /* a(1) corresponds to n-1 = 0, giving value 0 */
    idx_t k = n - 1;          /* binary counter (0,1,2,...) */
    seq_t value = 0;          /* interpreted in base 14 */
    seq_t power = 1;          /* 14^i  */
    while (k) {
        if (k & 1)           /* if current bit is 1, include this power of 14 */
            value += power;
        power *= (seq_t)14;  /* move to next digit position */
        k >>= 1;
    }
    return value * (seq_t)13; /* multiply by 13 to get digits {0,13} only */
}
