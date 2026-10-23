/*
 * A106706  a(0) = 19; for n>0, successively subtract 5, subtract 3 and
 *          double.
 * https://oeis.org/A106706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(3n) = 3*2^n+16; a(3n+1) = 3*2^n+11; a(3n+2) = 3*2^n+8
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 92, u64 n <= 188,
 *            u128 n <= 380, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t k = n / 3;
    seq_t p = pow2(k);          /* 2^k in seq_t */
    seq_t val = p * (seq_t)3;    /* 3*2^k */

    switch (n % 3) {
        case 0: return val + (seq_t)16;
        case 1: return val + (seq_t)11;
        case 2: return val + (seq_t)8;
        default: return (seq_t)0;   /* unreachable but keeps compiler happy */
    }
}
