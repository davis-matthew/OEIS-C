/*
 * A052389  Number of 4-element intersecting families (with not necessarily
 *          distinct sets) of an n-element set.
 * https://oeis.org/A052389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (16^n - 6*12^n + 12*10^n - 9^n -10*8^n + 15*7^n - 24*6^n
 *            + 19*5^n + 5*4^n - 11*3^n + 6*2^n - 6)/24 For 64‑bit builds the
 *            computation is done in 128‑bit to avoid intermediate overflow.
 *            For larger builds the generic big_pow routine is used
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 32,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 825 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

static seq_t a_128bit(idx_t n) {
    __int128 p16 = 1, p12 = 1, p10 = 1, p9 = 1, p8 = 1, p7 = 1, p6 = 1, p5 = 1, p4 = 1, p3 = 1, p2 = 1;
    for (idx_t i = 0; i < n; ++i) {
        p16 *= 16; p12 *= 12; p10 *= 10; p9 *= 9;
        p8 *= 8; p7 *= 7; p6 *= 6; p5 *= 5; p4 *= 4; p3 *= 3; p2 *= 2;
    }
    __int128 num =  p16
                  - (__int128)6 * p12
                  + (__int128)12 * p10
                  - p9
                  - (__int128)10 * p8
                  + (__int128)15 * p7
                  - (__int128)24 * p6
                  + (__int128)19 * p5
                  + (__int128)5  * p4
                  - (__int128)11 * p3
                  + (__int128)6  * p2
                  - 6;
    num /= 24;
    return (seq_t)num;
}

seq_t a(idx_t n) {
    /* Use 128‑bit internally for 64‑bit seq_t to avoid overflow.
       If seq_t is bigger (arbitrary precision), fall back to big_pow. */
#if defined(__SIZEOF_INT128__)
    if (sizeof(seq_t) == 8) {
        return a_128bit(n);
    }
#endif
    /* General case */
    seq_t p16 = ipow((seq_t)16, (seq_t)n);
    seq_t p12 = ipow((seq_t)12, (seq_t)n);
    seq_t p10 = ipow((seq_t)10, (seq_t)n);
    seq_t p9  = ipow((seq_t)9 , (seq_t)n);
    seq_t p8  = ipow((seq_t)8 , (seq_t)n);
    seq_t p7  = ipow((seq_t)7 , (seq_t)n);
    seq_t p6  = ipow((seq_t)6 , (seq_t)n);
    seq_t p5  = ipow((seq_t)5 , (seq_t)n);
    seq_t p4  = ipow((seq_t)4 , (seq_t)n);
    seq_t p3  = ipow((seq_t)3 , (seq_t)n);
    seq_t p2  = ipow((seq_t)2 , (seq_t)n);
    seq_t num =  p16
                - (__int128)6 * p12
                + (__int128)12 * p10
                - p9
                - (__int128)10 * p8
                + (__int128)15 * p7
                - (__int128)24 * p6
                + (__int128)19 * p5
                + (__int128)5  * p4
                - (__int128)11 * p3
                + (__int128)6  * p2
                - 6;
    return num / (seq_t)24;
}
