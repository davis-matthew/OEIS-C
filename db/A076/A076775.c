/*
 * A076775  Greatest common divisor of n and the binary representation of n
 *          interpreted decimally.
 * https://oeis.org/A076775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 32, u32 n <= 1952, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 107 terms through n = 8569 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Greatest common divisor of n and the binary representation of
   n interpreted decimally (OEIS: A076775).
   Formula: a(n) = gcd( n, decimal_value_of_binary_replication_of_n )  */
#include <stdint.h>
#include "../utils/gcd.h"

/* Return the decimal value of the binary representation of n. */
static seq_t decimal_of_binary(idx_t n)
{
    /* Most‑significant‑bit index (n > 0). */
    unsigned msb = 63 - __builtin_clzll(n);   /* clz returns number of leading zeros */
    unsigned bitlen = msb + 1;               /* number of bits needed to represent n */

    seq_t dec = 0;
    for (int i = msb; i >= 0; --i) {
        /* dec = dec * 10 + current bit */
        dec = dec * 10 + ((n >> i) & 1);
    }
    return dec;
}

seq_t a(idx_t n)
{
    seq_t dec = decimal_of_binary(n);
    return gcd(dec, (seq_t)n);
}
