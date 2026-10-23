/*
 * A034163  Number of partitions of n into 3 distinct parts from [ 1,20 ].
 * https://oeis.org/A034163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as a sum of three distinct
 *            integers from 1..20
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static table of counts for sums 0..57. 56 is max sum (20+19+18). */
    static seq_t cnt[58];
    static int init = 0;

    if (!init) {
        /* Zero initialization (redundant but explicit). */
        for (size_t i = 0; i < sizeof(cnt)/sizeof(cnt[0]); ++i)
            cnt[i] = 0;

        /* Count all distinct triples (a,b,c) with 1<=a<b<c<=20. */
        for (idx_t a = 1; a <= 20; ++a) {
            for (idx_t b = a + 1; b <= 20; ++b) {
                for (idx_t c = b + 1; c <= 20; ++c) {
                    idx_t s = a + b + c;           /* sum in 6..57 */
                    if (s < 58)                    /* safety guard */
                        cnt[s] += 1;
                }
            }
        }
        init = 1;
    }

    if (n < 58)            /* n fits in array bounds */
        return cnt[(size_t)n];
    else
        return (seq_t)0;
}
