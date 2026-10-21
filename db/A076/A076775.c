/*
 * A076775  Greatest common divisor of n and the binary representation of n
 *          interpreted decimally.
 * https://oeis.org/A076775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 32, u32 n <= 1952, u64 n <= 8569,
 *            u128 n <= 8569, big any n
 * Helpers:   decimal_of_binary, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Greatest common divisor of n and the binary representation of
   n interpreted decimally (OEIS: A076775).
   Formula: a(n) = gcd( n, decimal_value_of_binary_replication_of_n )  */
#include "../utils/gcd.h"
#include "../utils/decimal_of_binary.h"

seq_t a(idx_t n)
{
    seq_t dec = decimal_of_binary(n);
    return gcd(dec, (seq_t)n);
}
