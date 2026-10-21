/*
 * A059946  Number of 5-block bicoverings of an n-set.
 * https://oeis.org/A059946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/120) * (10^n - 5·6^n - 10·4^n + 20·3^n + 30·2^n - 60)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 39,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below offset, unused */

    /* 64‑bit builds: use 128‑bit intermediate which is enough for all
       values that fit in 64 bits (n ≤ 21). */
    if (sizeof(seq_t) <= 8) {
        __uint128_t p2 = 1, p3 = 1, p4 = 1, p6 = 1, p10 = 1;
        for (idx_t i = 0; i < n; ++i) {
            p2  *= 2;
            p3  *= 3;
            p4  *= 4;
            p6  *= 6;
            p10 *= 10;
        }
        __uint128_t term = p10
                          - p6 * 5
                          - p4 * 10
                          + p3 * 20
                          + p2 * 30
                          - 60;
        return (seq_t)(term / 120);
    }

    /* Arbitrary‑precision build: use big_pow for exponentiation. */
    seq_t pow2  = big_pow((seq_t)2, (seq_t)n);
    seq_t pow3  = big_pow((seq_t)3, (seq_t)n);
    seq_t pow4  = big_pow((seq_t)4, (seq_t)n);
    seq_t pow6  = big_pow((seq_t)6, (seq_t)n);
    seq_t pow10 = big_pow((seq_t)10, (seq_t)n);

    seq_t term = pow10;
    term -= pow6 * (seq_t)5;
    term -= pow4 * (seq_t)10;
    term += pow3 * (seq_t)20;
    term += pow2 * (seq_t)30;
    term -= (seq_t)60;
    term /= (seq_t)120;

    return term;
}
