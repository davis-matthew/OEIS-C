/*
 * A184046  1/9 the number of (n+1) X 8 0..2 arrays with all 2 X 2 subblocks
 *          having the same four values.
 * https://oeis.org/A184046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*2^(n/2) + 2^(n+1) + 281 for n even, * a(n) = 2^(n+1) +
 *            2^((n+3)/2) + 281 for n odd
 * Range:     u8 none, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"   /* Provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* behaviour for offset < 1 is irrelevant */

    if ((n & 1) == 0) {          /* n is even */
        seq_t part1 = (seq_t)3 * pow2(n / 2);
        seq_t part2 = pow2(n + 1);
        return part1 + part2 + (seq_t)281;
    } else {                     /* n is odd */
        seq_t part1 = pow2(n + 1);
        seq_t part2 = pow2((n + 3) / 2);
        return part1 + part2 + (seq_t)281;
    }
}
