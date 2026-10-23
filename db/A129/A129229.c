/*
 * A129229  a(n) = floor(n*r)-a(n-1), where r is the golden mean,
 *          (1+sqrt(5))/2.
 * https://oeis.org/A129229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*r) - a(n-1), r = (1+sqrt(5))/2, with a(0)=0
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t prev = 0;          /* a(0) */
    seq_t curr = 0;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t mi = (seq_t)i;
        seq_t sq = mi * mi;              /* mi² */
        seq_t sq5 = sq * 5;              /* 5·mi² */
        seq_t sqrt_val = floor_sqrt(sq5);/* floor(sqrt(5·mi²)) */
        seq_t floor_phi = (mi + sqrt_val) / 2; /* floor(mi*r) */

        curr = floor_phi - prev;        /* a(i) = floor(i*r) - a(i-1) */
        prev = curr;
    }

    return curr;
}
