/*
 * A000838  Number of n-input 2-output switching networks under action of
 *          complementing group on the inputs and outputs.
 * https://oeis.org/A000838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(2^(n+1) - 2 - n) + 2^(2^n) - 2^(2^n - n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   a838_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a838_pow2.h"

seq_t a(idx_t n) {
    if (n >= 62) return 0; /* value far exceeds any representable size */
    uint64_t p = (uint64_t)1 << n;          /* 2^n */
    uint64_t e1 = 2 * p - 2 - (uint64_t)n;  /* 2^(n+1) - 2 - n */
    uint64_t e2 = p;                        /* 2^n */
    uint64_t e3 = p - (uint64_t)n;          /* 2^n - n */
    seq_t t1 = a838_pow2(e1);
    seq_t t2 = a838_pow2(e2);
    seq_t t3 = a838_pow2(e3);
    seq_t r = t1 + t2;
    r = r - t3;
    return r;
}
