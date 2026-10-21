/*
 * A051304  Number of 4-element proper antichains of an n-element set.
 * https://oeis.org/A051304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (16^n -18*12^n +60*10^n -9*9^n -102*8^n +105*7^n -90*6^n
 *            +95*5^n -31*4^n -33*3^n +28*2^n -6)/24
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 17, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Small seq_t (≤ 8 bytes) – use 128‑bit intermediate */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 p16 = 1, p12 = 1, p10 = 1, p9  = 1,
                         p8  = 1, p7  = 1, p6  = 1, p5  = 1,
                         p4  = 1, p3  = 1, p2  = 1;
        for (idx_t i = 0; i < n; ++i) {
            p16 *= 16; p12 *= 12; p10 *= 10; p9  *= 9;
            p8  *= 8;  p7  *= 7;  p6  *= 6;  p5  *= 5;
            p4  *= 4;  p3  *= 3;  p2  *= 2;
        }
        unsigned __int128 pos = 0, neg = 0;
        pos += p16;
        pos += p10 * 60;
        pos += p7  * 105;
        pos += p5  * 95;
        pos += p2  * 28;
        neg += p12 * 18;
        neg += p9  * 9;
        neg += p8  * 102;
        neg += p6  * 90;
        neg += p4  * 31;
        neg += p3  * 33;
        neg += 6;
        unsigned __int128 res = (pos - neg) / 24;
        return (seq_t)res;
    }
    /* Arbitrary‑precision seq_t – use big_pow */
    seq_t p16 = big_pow((seq_t)16, (seq_t)n);
    seq_t p12 = big_pow((seq_t)12, (seq_t)n);
    seq_t p10 = big_pow((seq_t)10, (seq_t)n);
    seq_t p9  = big_pow((seq_t)9,  (seq_t)n);
    seq_t p8  = big_pow((seq_t)8,  (seq_t)n);
    seq_t p7  = big_pow((seq_t)7,  (seq_t)n);
    seq_t p6  = big_pow((seq_t)6,  (seq_t)n);
    seq_t p5  = big_pow((seq_t)5,  (seq_t)n);
    seq_t p4  = big_pow((seq_t)4,  (seq_t)n);
    seq_t p3  = big_pow((seq_t)3,  (seq_t)n);
    seq_t p2  = big_pow((seq_t)2,  (seq_t)n);

    seq_t pos = 0;
    pos += p16;
    pos += p10 * (seq_t)60;
    pos += p7  * (seq_t)105;
    pos += p5  * (seq_t)95;
    pos += p2  * (seq_t)28;

    seq_t neg = 0;
    neg += p12 * (seq_t)18;
    neg += p9  * (seq_t)9;
    neg += p8  * (seq_t)102;
    neg += p6  * (seq_t)90;
    neg += p4  * (seq_t)31;
    neg += p3  * (seq_t)33;
    neg += (seq_t)6;

    seq_t res = (pos - neg) / (seq_t)24;
    return res;
}
