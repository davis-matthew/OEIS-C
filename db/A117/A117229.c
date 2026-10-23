/*
 * A117229  Number of decimal digits of n in {0,1,4,8,9}.
 * https://oeis.org/A117229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of decimal digits of n that are in {0,1,4,8,9}
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    unsigned char cnt[10];
    digit_counts((seq_t)n, cnt);        /* count each decimal digit of n */
    return (seq_t)(cnt[0] + cnt[1] + cnt[4] + cnt[8] + cnt[9]); /* sum the counts for the digits 0,1,4,8,9 */
}
