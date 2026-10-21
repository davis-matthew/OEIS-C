/*
 * A056968  10^(n-1) modulo n.
 * https://oeis.org/A056968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = 10^(n-1) mod n (OEIS A056968)
 * Range:     u8 n <= 278, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* n is at least 1 according to the offset */
    if (n == 0) return 0;                     /* safety, though not used */
    return mod_pow((seq_t)10, n - 1, (seq_t)n); /* fast exponentiation modulo n */
}
