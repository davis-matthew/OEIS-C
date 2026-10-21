/*
 * A082061  Greatest common prime divisor of n and phi(n)=A000010(n); a(n)=1
 *          if no common prime divisor exists.
 * https://oeis.org/A082061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime divisor of gcd(n, φ(n)); 1 if gcd = 1
 * Range:     u8 n <= 548, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   largest_prime_factor_uint64, phi_calc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes φ(n) by trial division; works for any n ≥ 1 within the limits
   required. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/largest_prime_factor_uint64.h"
#include "../utils/phi_calc.h"

/* Euclidean algorithm for 64‑bit unsigned integers. */
static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)          /* offset starts at 1; return 1 for non‑positive indices. */
        return 1;

    seq_t phi_val = phi_calc(n);
    uint64_t g = gcd_uint64((uint64_t)n, (uint64_t)phi_val);
    return largest_prime_factor_uint64(g);
}
