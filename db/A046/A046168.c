/*
 * A046168  Number of minimal covers on n objects with 8 members.
 * https://oeis.org/A046168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (1/8!) * (255^n - 8*254^n + 28*253^n - 56*252^n +
 *            70*251^n - 56*250^n + 28*249^n - 8*248^n + 247^n)
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 11, u64 n <= 14, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: exponentiation by squaring for seq_t */
static seq_t seq_pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n < 8) return 0;
    if (n == 8) return 1;

    /* For small n (≤15) we use 128‑bit arithmetic to avoid overflow
       of the 64‑bit seq_t used in the 64‑bit build. */
    if (n <= 15)
    {
#if defined(__SIZEOF_INT128__)
        typedef unsigned __int128 u128;
        u128 pow255 = 1, pow254 = 1, pow253 = 1, pow252 = 1, pow251 = 1;
        u128 pow250 = 1, pow249 = 1, pow248 = 1, pow247 = 1;
        for (idx_t i = 1; i <= n; ++i)
        {
            pow255 *= (u128)255;
            pow254 *= (u128)254;
            pow253 *= (u128)253;
            pow252 *= (u128)252;
            pow251 *= (u128)251;
            pow250 *= (u128)250;
            pow249 *= (u128)249;
            pow248 *= (u128)248;
            pow247 *= (u128)247;
        }
        u128 sum = pow255
                 - (u128)8   * pow254
                 + (u128)28  * pow253
                 - (u128)56  * pow252
                 + (u128)70  * pow251
                 - (u128)56  * pow250
                 + (u128)28  * pow249
                 - (u128)8   * pow248
                 + pow247;
        /* 8! = 40320 */
        seq_t result = (seq_t)(sum / (u128)40320);
        return result;
#else
        /* Fallback if __int128 is not available: use seq_t, assuming n is so small
           that 64‑bit overflow does not occur. */
        seq_t pow255 = 1, pow254 = 1, pow253 = 1, pow252 = 1, pow251 = 1;
        seq_t pow250 = 1, pow249 = 1, pow248 = 1, pow247 = 1;
        for (idx_t i = 1; i <= n; ++i)
        {
            pow255 *= (seq_t)255;
            pow254 *= (seq_t)254;
            pow253 *= (seq_t)253;
            pow252 *= (seq_t)252;
            pow251 *= (seq_t)251;
            pow250 *= (seq_t)250;
            pow249 *= (seq_t)249;
            pow248 *= (seq_t)248;
            pow247 *= (seq_t)247;
        }
        seq_t sum = pow255
                  - (seq_t)8   * pow254
                  + (seq_t)28  * pow253
                  - (seq_t)56  * pow252
                  + (seq_t)70  * pow251
                  - (seq_t)56  * pow250
                  + (seq_t)28  * pow249
                  - (seq_t)8   * pow248
                  + pow247;
        return sum / (seq_t)40320;
#endif
    }

    /* General case for large n: use seq_t big‑integer arithmetic. */
    seq_t pow255 = seq_pow_seq((seq_t)255, n);
    seq_t pow254 = seq_pow_seq((seq_t)254, n);
    seq_t pow253 = seq_pow_seq((seq_t)253, n);
    seq_t pow252 = seq_pow_seq((seq_t)252, n);
    seq_t pow251 = seq_pow_seq((seq_t)251, n);
    seq_t pow250 = seq_pow_seq((seq_t)250, n);
    seq_t pow249 = seq_pow_seq((seq_t)249, n);
    seq_t pow248 = seq_pow_seq((seq_t)248, n);
    seq_t pow247 = seq_pow_seq((seq_t)247, n);

    seq_t sum = pow255
              - (seq_t)8   * pow254
              + (seq_t)28  * pow253
              - (seq_t)56  * pow252
              + (seq_t)70  * pow251
              - (seq_t)56  * pow250
              + (seq_t)28  * pow249
              - (seq_t)8   * pow248
              + pow247;

    return sum / (seq_t)40320;
}
