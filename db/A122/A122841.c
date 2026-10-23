/*
 * A122841  Greatest k such that 6^k divides n.
 * https://oeis.org/A122841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min(v2(n), v3(n)) – the greatest k such that 6^k divides
 *            n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t if needed, but idx_t is provided */

seq_t a(idx_t n) {
    /* Special case: n == 0 is undefined for OEIS; return 0 */
    if (n == 0) return (seq_t)0;

    idx_t tmp = n;
    unsigned int count2 = 0;
    while ((tmp & 1) == 0) {   /* faster MOD 2 test */
        tmp >>= 1;
        count2++;
    }

    tmp = n;
    unsigned int count3 = 0;
    while (tmp % 3 == 0) {
        tmp /= 3;
        count3++;
    }

    unsigned int k = count2 < count3 ? count2 : count3;
    return (seq_t)k;
}
