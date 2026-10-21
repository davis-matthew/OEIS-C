/*
 * A081770  Numbers twice their squarefree kernel (A007947).
 * https://oeis.org/A081770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * rad(n), where rad(n) is the squarefree kernel of n.
 *            Sequence: numbers equal to twice their squarefree kernel
 * Range:     u8 n <= 26, u16 n <= 6641, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rad_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/rad_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* safety for offset 0 */

    idx_t cnt = 0;
    uint64_t x   = 2;                       /* first integer to test */

    while (1) {
        seq_t rad  = rad_uint64(x);
        seq_t twice = ((seq_t)2) * rad;
        if ((seq_t)x == twice) {
            ++cnt;
            if (cnt == n) return (seq_t)x;
        }
        ++x;
    }
}
