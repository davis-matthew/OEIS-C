/*
 * A043278  Maximal run length in base 4 representation of n.
 * https://oeis.org/A043278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = max run length of equal digits in the base‑4
 *            representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* not used in the sequence but safe */
    idx_t max_run = 0;
    idx_t cur_run = 0;
    int    prev = -1;                    /* sentinel value for first digit */

    idx_t m = n;
    while (m > 0) {
        int digit = m % 4;                /* extract lowest base‑4 digit */
        if (digit == prev) {
            ++cur_run;
        } else {
            cur_run = 1;
            prev = digit;
        }
        if (cur_run > max_run) max_run = cur_run;
        m /= 4;                           /* shift to next digit */
    }
    return (seq_t)max_run;
}
