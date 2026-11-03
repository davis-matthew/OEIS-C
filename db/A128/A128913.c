/*
 * A128913  a(n) = n*pi(n).
 * https://oeis.org/A128913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * pi(n)
 * Range:     u8 n <= 28, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1 starting at offset 1. */
    if (n == 0)
        return 0; /* safety for unconventional calls */
    return (seq_t)n * prime_pi(n);
}
