/*
 * A071354  Floor(2^n/n) is odd.
 * https://oeis.org/A071354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer k for which floor(2^k / k) is odd
   Sequence A071354: 12,18,25,36,42,45,48,55,80,91,...
   a(n) is the n-th such integer, offset 1.
*/

#include <stdint.h>

static uint64_t pow2_mod_uint64(uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t base   = 2 % mod;
    while (exp > 0) {
        if (exp & 1U)
            result = (uint64_t)((__uint128_t)result * base % mod);
        base = (uint64_t)((__uint128_t)base * base % mod);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        uint64_t mod = 2 * k;              /* 2*k fits in uint64_t for k <= 9e18 */
        uint64_t r   = pow2_mod_uint64(k, mod);
        if (r >= k) {                      /* floor(2^k / k) odd ⇔ r ≥ k */
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
