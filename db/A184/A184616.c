/*
 * A184616  Negated negative parts of the nonadjacent forms.
 * https://oeis.org/A184616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of 2^i for positions i where the nonadjacent form of
 *            n has digit -1. The nonadjacent form (NAF) of n is generated
 *            using the standard algorithm: if n is odd, choose the digit d=1
 *            when n mod 4 = 1, otherwise d=-1; set n := (n-d)/2. Whenever d =
 *            -1, add the current 2^i to the result
 * Range:     u8 n <= 682, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t m = n;
    seq_t sum = 0;
    seq_t power = 1;   /* 2^i, starting with i = 0 */

    while (m > 0) {
        if (m & 1) {            /* n is odd */
            int d = ((m & 3) == 1) ? 1 : -1;   /* choose d = +1 or -1 */
            if (d == -1) {
                sum += power;                       /* accumulate negative part */
                m = (m + 1) >> 1;                    /* (m - d) / 2  with d = -1 */
            } else {                               /* d == +1 */
                m = (m - 1) >> 1;                    /* (m - d) / 2  with d = +1 */
            }
        } else {                           /* n is even */
            m >>= 1;                       /* (m - 0) / 2 */
        }
        power <<= 1;                       /* move to next bit */
    }
    return sum;
}
