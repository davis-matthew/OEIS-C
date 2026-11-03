/*
 * A100634  a(n) is the decimal equivalent of the binary number whose k-th
 *          least significant bit is 1 iff k is a prime number and k <= n.
 * https://oeis.org/A100634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{p ≤ n, p prime} 2^(p‑1) (k‑th least significant bit is
 *            1 iff k is prime)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 36, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 279 (2026-11-03)
 * Helpers:   is_prime_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    if (n < 2) return sum;          /* a(1) = 0 */
    for (idx_t p = 2; p <= n; ++p) {
        if (is_prime_u64(p)) {
            sum += pow2(p - 1);
        }
    }
    return sum;
}
