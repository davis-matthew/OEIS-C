/*
 * A167482  a(n)=n-1 if n is prime, otherwise a(n)=n+1.
 * https://oeis.org/A167482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-1 if n is prime, otherwise a(n) = n+1
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (is_prime(n))
        return (seq_t)n - 1;
    else
        return (seq_t)n + 1;
}
