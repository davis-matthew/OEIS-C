/*
 * A100690  a(n) = p * 5^p - 1 where p=prime(n).
 * https://oeis.org/A100690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p * 5^p - 1, where p = prime(n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 216 (2026-11-03)
 * Helpers:   ipow_idx, prime_nth
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A100690: a(n) = p 5^p - 1 with p = prime(n), from the shared prime table
 * (the original's cache appended a second copy of the primes when it grew). */

#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/prime_nth.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint64_t p = prime_nth((uint64_t)n);
    return ipow_idx((seq_t)5, (idx_t)p) * (seq_t)p - (seq_t)1;
}
