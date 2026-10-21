/*
 * A212844  a(n) = 2^(n+2) mod n.
 * https://oeis.org/A212844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n+2) mod n
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Guard against a zero index, although the sequence is defined for n >= 1 */
    if (n == 0) return 0;

    /* Compute 2^(n+2) mod n using the provided modular exponentiation routine. */
    return mod_pow((seq_t)2, (uint64_t)(n + 2), (seq_t)n);
}
