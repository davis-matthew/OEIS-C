/*
 * A056142  Concatenate n, floor[n/10], floor[n/100] ... (but do not continue
 *          if floor[.]=0).
 * https://oeis.org/A056142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concatenate n, floor(n/10), floor(n/100), ... until
 *            floor(..)=0
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Special case n = 0 */
    if (n == 0) return (seq_t)0;

    /* Precompute powers of 10 once */
    static seq_t pow10[21];
    static int init = 0;
    if (!init) {
        pow10[0] = (seq_t)1;
        for (int i = 1; i < 21; ++i) {
            pow10[i] = pow10[i - 1] * (seq_t)10;
        }
        init = 1;
    }

    seq_t cur = (seq_t)n;
    seq_t result = (seq_t)0;

    while (cur > 0) {
        /* Count decimal digits of cur */
        seq_t tmp = cur;
        int d = 0;
        while (tmp > 0) {
            tmp /= (seq_t)10;
            ++d;
        }

        /* Append cur: result = result * 10^d + cur */
        result = result * pow10[d] + cur;

        /* Move to next truncation */
        cur /= (seq_t)10;
    }
    return result;
}
