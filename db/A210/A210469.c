/*
 * A210469  a(n) = n - primepi(2n).
 * https://oeis.org/A210469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - primepi(2n)
 * Range:     u8 n <= 392, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9720 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    return (seq_t)n - prime_pi(n * (idx_t)2);
}
