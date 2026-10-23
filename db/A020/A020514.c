/*
 * A020514  a(n) = 1^n + 2^n + 4^n + 8^n + 16^n.
 * https://oeis.org/A020514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1^n + 2^n + 4^n + 8^n + 16^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t result = 1;                       /* 1^n = 1 */
    seq_t p1 = pow2(n);                     /* 2^n  */
    seq_t p2 = pow2(2 * n);                 /* 4^n = 2^(2n) */
    seq_t p3 = pow2(3 * n);                 /* 8^n = 2^(3n) */
    seq_t p4 = pow2(4 * n);                 /* 16^n = 2^(4n) */
    result += p1;
    result += p2;
    result += p3;
    result += p4;
    return result;
}
