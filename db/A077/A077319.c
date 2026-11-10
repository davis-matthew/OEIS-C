/*
 * A077319  Average of terms in n-th row of A077316.
 * https://oeis.org/A077319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A077318(n)/n, where A077318(n) = sum of the n smallest
 *            primes == 1 (mod n); computed by search: primes n*k+1 give a(n)
 *            = 1 + sum of their k values
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    seq_t sum = 1;          /* each prime is n*k+1, so the average is 1 + (sum of k) */
    idx_t found = 0;
    uint64_t k = 1;
    while (found < n) {
        uint64_t cand = (uint64_t)n * k + 1;
        if (is_prime_u64(cand)) {
            sum += (seq_t)k;
            ++found;
        }
        ++k;
    }
    return sum;
}
