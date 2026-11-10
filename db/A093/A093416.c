/*
 * A093416  Index of first occurrence of prime(n) in A034694.
 * https://oeis.org/A093416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that A034694(k) = prime(n), i.e. the
 *            least divisor k of prime(n)-1 such that no prime q < prime(n)
 *            has q == 1 (mod k). Found by search
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-07)
 * Helpers:   is_prime_u64, nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/nth_prime.h"

seq_t a(idx_t n)
{
    uint64_t p = nth_prime(n);
    for (uint64_t k = 1; k < p; k++) {
        if ((p - 1) % k) continue;
        bool ok = true;
        for (uint64_t q = k + 1; q < p; q += k) {
            if (is_prime_u64(q)) { ok = false; break; }
        }
        if (ok) return (seq_t)k;
    }
    return (seq_t)1;
}
