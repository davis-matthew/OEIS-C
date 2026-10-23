/*
 * A039033  Numbers whose base-7 representation has the same number of 0's and
 *          4's.
 * https://oeis.org/A039033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth natural number whose base-7 representation
 *            contains an equal number of 0's and 4's
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* check whether a number has equal counts of 0's and 4's in base 7 */
static bool equal_zero_four(seq_t n)
{
    seq_t t = n;
    unsigned int cnt0 = 0, cnt4 = 0;
    while (t != 0) {
        unsigned int digit = (unsigned int)(t % 7);
        if (digit == 0) cnt0++;
        else if (digit == 4) cnt4++;
        t = t / 7;
    }
    return cnt0 == cnt4;
}

seq_t a(idx_t n)
{
    seq_t num = 1;
    idx_t found = 0;
    while (found < n) {
        if (equal_zero_four(num)) {
            found++;
            if (found == n) return num;
        }
        num++;
    }
    /* unreachable, but a return is required for strict conformance */
    return num;
}
