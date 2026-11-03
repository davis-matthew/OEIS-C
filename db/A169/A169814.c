/*
 * A169814  a(n) = n XOR phi(n).
 * https://oeis.org/A169814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n XOR phi(n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9466 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

/* Compute a(n) = n XOR phi(n) for n >= 1 */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;   /* Convert n to seq_t */
    seq_t ph = totient(n);     /* Euler's totient of n */
    return sn ^ ph;        /* Bitwise XOR in seq_t arithmetic */
}
