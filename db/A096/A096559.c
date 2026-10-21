/*
 * A096559  Consecutive states of a linear congruential pseudo-random number
 *          generator that has the spectrally best primitive root for 2^31-1
 *          as multiplier.
 * https://oeis.org/A096559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 62089911^(n-1) mod (2^31-1), a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is undefined for n < 1.  Return 0 in that case. */
    if (n < 1)
        return 0;

    /* 2^31 - 1 = 2147483647, the modulus for the linear congruential
     * generator.  The multiplier is 62089911. */
    const seq_t MOD = (seq_t)2147483647ULL;
    const seq_t MUL = (seq_t)62089911ULL;

    /* a(1) = 1, and for n > 1  a(n) = MUL * a(n-1) mod MOD.
     * Hence a(n) = MUL^(n-1) mod MOD.  Use the efficient modular
     * exponentiation routine from utils/mod_pow.h. */
    return mod_pow(MUL, n - 1, MOD);
}
