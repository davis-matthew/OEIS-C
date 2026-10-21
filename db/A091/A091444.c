/*
 * A091444  Concatenate binary vectors ordered first by length, then by the
 *          number of 1's and finally lexicographically.
 * https://oeis.org/A091444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9717, u16 n <= 9717, u32 n <= 9717, u64 n <= 9717,
 *            u128 n <= 9717, big any n
 * Helpers:   popcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes a(n) as the nth bit of the infinite binary stream
   formed by concatenating all binary vectors, first sorted by length,
   then by number of 1's, then lexicographically. */

#include <stdint.h>
#include "../utils/popcount.h"

seq_t a(idx_t n) {
    idx_t cum = 0;                     /* number of bits already processed */
    for (idx_t len = 1; ; ++len) {     /* current vector length */
        idx_t max = ((idx_t)1 << len); /* 2^len */
        for (idx_t w = 0; w <= len; ++w) { /* number of 1's */
            for (idx_t x = 0; x < max; ++x) {
                if (popcount(x) != w) continue; /* skip wrong popcount */
                if (cum + len > n) {            /* the desired bit is here */
                    idx_t pos = n - cum;        /* position inside this vector */
                    idx_t bit = (x >> (len - 1 - pos)) & 1;
                    return (seq_t)bit;
                }
                cum += len;                    /* move past this vector */
            }
        }
    }
    /* Unreachable, but keeps the compiler happy. */
    return 0;
}
