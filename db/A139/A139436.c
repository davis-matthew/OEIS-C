/*
 * A139436  Number of ways to write n as sum of initial terms of a Collatz
 *          trajectory starting with a value less than n.
 * https://oeis.org/A139436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as a sum of the initial * terms
 *            of a Collatz trajectory that starts with a value less than n
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) return 0;                /* no m < n when n is 1 */
    seq_t count  = 0;
    const seq_t target = (seq_t)n;

    for (idx_t m = 1; m < n; ++m) {
        seq_t sum = 0;
        idx_t cur = m;

        while (sum < target) {
            sum += (seq_t)cur;
            if (sum == target) {
                ++count;
                break;           /* further terms will only increase sum */
            }
            /* Collatz step */
            if ((cur & 1ULL) == 0)
                cur >>= 1;      /* even */
            else
                cur = cur * 3 + 1; /* odd */
        }
    }
    return count;
}
