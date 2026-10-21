/*
 * A221556  Consecutive values produced by the C++ minstd_rand random number
 *          generator with the default seed (1).
 * https://oeis.org/A221556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 48271^n mod (2^31-1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The modulus 2^31-1 fits in a 32‑bit unsigned integer. */
    const seq_t MOD  = (seq_t)2147483647;   /* 0x7FFFFFFF */
    const seq_t BASE = (seq_t)48271;        /* multiplier */

    seq_t result = (seq_t)1;
    seq_t power  = BASE % MOD;   /* base^1 mod MOD */
    idx_t exp    = n;           /* exponent (n >= 1) */

    while (exp) {
        if (exp & 1U)           /* if the lowest bit of exp is 1 */
            result = (result * power) % MOD;

        exp >>= 1U;            /* shift exponent right by one bit */

        if (exp)                /* avoid an extra square when exp becomes 0 */
            power = (power * power) % MOD;
    }
    return result;              /* result = 48271^n mod MOD */
}
