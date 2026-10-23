/*
 * A048439  Take the first n numbers written in base 7, concatenate them, then
 *          convert from base 7 to base 10.
 * https://oeis.org/A048439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation of the base‑7 representations of 1,2,…,n,
 *            interpreted as a base‑7 number
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* compute number of base‑7 digits of k */
        idx_t tmp = k;
        idx_t len = 1;
        while (tmp >= 7) {
            tmp /= 7;
            ++len;
        }
        /* multiply current result by 7^len */
        for (idx_t i = 0; i < len; ++i)
            res *= (seq_t)7;
        /* add k (its base‑7 digits have already been accounted for) */
        res += (seq_t)k;
    }
    return res;
}
