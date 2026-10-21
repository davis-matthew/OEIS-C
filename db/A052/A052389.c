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
 * Helpers:   a_128bit, big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/a_128bit.h"

seq_t a(idx_t n) {
    /* Use 128‑bit internally for 64‑bit seq_t to avoid overflow.
       If seq_t is bigger (arbitrary precision), fall back to big_pow. */
#if defined(__SIZEOF_INT128__)
    if (sizeof(seq_t) == 8) {
        return a_128bit(n);
    }
#endif
    /* General case */
    seq_t p16 = big_pow((seq_t)16, (seq_t)n);
    seq_t p12 = big_pow((seq_t)12, (seq_t)n);
    seq_t p10 = big_pow((seq_t)10, (seq_t)n);
    seq_t p9  = big_pow((seq_t)9 , (seq_t)n);
    seq_t p8  = big_pow((seq_t)8 , (seq_t)n);
    seq_t p7  = big_pow((seq_t)7 , (seq_t)n);
    seq_t p6  = big_pow((seq_t)6 , (seq_t)n);
    seq_t p5  = big_pow((seq_t)5 , (seq_t)n);
    seq_t p4  = big_pow((seq_t)4 , (seq_t)n);
    seq_t p3  = big_pow((seq_t)3 , (seq_t)n);
    seq_t p2  = big_pow((seq_t)2 , (seq_t)n);
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
