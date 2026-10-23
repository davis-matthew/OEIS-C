/*
 * A046059  Orders of finite groups having the incrementally largest numbers
 *          of nonisomorphic forms A046058.
 * https://oeis.org/A046059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   a(1)=1, a(2)=4, a(3)=8, a(4)=16, a(5)=24, a(6)=32, a(7)=48,
 *            a(n)=2^(n-2) for n>=8 (OEIS A046059)
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 4;
    if (n == 3) return 8;
    if (n == 4) return 16;
    if (n == 5) return 24;
    if (n == 6) return 32;
    if (n == 7) return 48;
    /* n >= 8 */
    return pow2(n - 2);
}
