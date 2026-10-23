/*
 * A159981  Catalan numbers read modulo 4.
 * https://oeis.org/A159981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A000108(n) mod 4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   trailing_zeros64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/trailing_zeros64.h"

static unsigned popcount64(idx_t x) {
    unsigned c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n) {
    unsigned p = popcount64(n);        /* number of 1-bits in n      */
    idx_t     m = n + 1;               /* n + 1 (may overflow)       */
    unsigned e = trailing_zeros64(m);  /* v2(n + 1)                   */
    int v = (int)p - (int)e;           /* v2(C_n)                     */
    if (v >= 2) return (seq_t)0;       /* 2^2 or more -> 0 mod 4      */
    if (v == 1) return (seq_t)2;       /* 2^1  -> 2 mod 4             */
    return (seq_t)1;                   /* odd -> 1 mod 4              */
}
