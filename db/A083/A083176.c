/*
 * A083176  Arithmetic means of rows of A083173.
 * https://oeis.org/A083176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)*(n+1)/2 if n is odd, prime(n)*(n+2)/2 if n is
 *            even (= A083175(n)/n)
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-07)
 * Helpers:   nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/nth_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t p = (seq_t)nth_prime(n);
    seq_t m = (seq_t)n;
    seq_t f = (n & 1) ? (m + 1) / 2 : (m + 2) / 2;
    return p * f;
}
