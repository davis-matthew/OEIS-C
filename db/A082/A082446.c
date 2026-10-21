/*
 * A082446  a(0)=0, a(1)=1, a(2)=0; thereafter, if k>=0 and a block of the
 *          first 3*2^k terms is known, then a(3*2^k+i)=1-a(i) for 0<=i<3*2^k.
 * https://oeis.org/A082446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (hammingweight(n/3) + (n % 3)) % 2
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   popcount_parity_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/popcount_parity_uint64.h"

seq_t a(idx_t n)
{
    uint64_t div3 = n / 3;              /* n divided by 3 */
    unsigned pop_parity = popcount_parity_uint64(div3);  /* parity of popcount(div3) */
    unsigned mod3_parity = (unsigned)(n % 3) & 1;        /* parity of (n % 3) */
    unsigned res = pop_parity ^ mod3_parity;             /* (pop + mod3) % 2 */
    return (seq_t)res;
}
