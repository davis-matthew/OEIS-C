/*
 * A213204  Number of terms of the Collatz (3x+1) trajectory of n that are
 *          greater than or equal to n.
 * https://oeis.org/A213204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of terms of the Collatz (3x+1) trajectory of n
 *            that are >= n
 * Range:     u8 n <= 26, u16 n <= 4510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t curr   = (seq_t)n;
    seq_t nval   = (seq_t)n;

    while (1) {
        if (curr >= nval) {
            count++;
        }
        if (curr == 1) {
            break;
        }
        if ((curr % 2) == 0) {
            curr = curr / 2;
        } else {
            curr = curr * 3 + 1;
        }
    }
    return count;
}
