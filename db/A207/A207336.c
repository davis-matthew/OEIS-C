/*
 * A207336  One half of smallest positive nontrivial even solution of the
 *          congruence x^2 == 1 (mod A001748(n+2)), n>=1.
 * https://oeis.org/A207336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*prime(n+2) - A208296(n))/2. Computed via CRT: the two
 *            non‑trivial solutions of x²≡1 (mod 3*prime(n+2)) are x ≡ 1 (mod
 *            3), x ≡ p‑1 (mod p) and x ≡ 2 (mod 3), x ≡ 1 (mod p), where p =
 *            prime(n+2). The smallest even solution divided by 2 gives a(n)
 * Range:     u8 n <= 95, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, mod_inv_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/mod_inv_uint64.h"

seq_t a(idx_t n)
{
    /* n >= 1 */
    uint64_t p = (uint64_t) get_prime(n + 2);  /* prime(n+2) */
    uint64_t inv3 = mod_inv_uint64(3ULL, p);  /* inverse of 3 mod p */

    /* First non‑trivial solution: x ≡ 1 (mod 3), x ≡ p‑1 (mod p) */
    uint64_t k1 = ((p - 2) * inv3) % p;
    uint64_t x1 = 3 * k1 + 1;

    /* Second non‑trivial solution: x ≡ 2 (mod 3), x ≡ 1 (mod p) */
    uint64_t k2 = ((p - 1) * inv3) % p;
    uint64_t x2 = 3 * k2 + 2;

    /* Choose the smallest even solution */
    bool x1_even = ((x1 & 1ULL) == 0);
    bool x2_even = ((x2 & 1ULL) == 0);

    uint64_t chosen;
    if (x1_even && !x2_even) {
        chosen = x1;
    } else if (!x1_even && x2_even) {
        chosen = x2;
    } else if (x1_even && x2_even) {
        chosen = (x1 < x2 ? x1 : x2);
    } else { /* unlikely: both odd, pick the smaller anyway */
        chosen = (x1 < x2 ? x1 : x2);
    }

    /* Return half of the chosen even solution */
    return (seq_t)(chosen >> 1);
}
