/*
 * A092188  a(n) = smallest positive integer m such that 2^3^4^5^...^n == m
 *          (mod n).
 * https://oeis.org/A092188
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, gcd, is_power_of_two, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n if n is a power of two;
 *            otherwise a(n) = (2^3^4^5^...^n) mod n
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_power_of_two.h"
#include "../utils/mod_pow.h"
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

/* Compute Euler's totient φ(n) for a 64‑bit unsigned integer n.
   Uses factor_u64 to factor n into primes, then applies the product
   formula for the totient. */
static uint64_t phi_uint64(uint64_t n)
{
    if (n <= 1) return 1;
    uint64_t result = n;
    uint64_t primes[64];
    uint64_t exps[64];        /* exponents are not needed here */
    size_t   cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        result = result / primes[i] * (primes[i] - 1);
    }
    return result;
}

/* Recursive evaluation of the power tower  start^(start+1^...^end)
   taken modulo mod.  The argument mod is converted to a 64‑bit
   integer for totient calculations; result is returned in seq_t. */
static seq_t tower_mod_rec(idx_t start, idx_t end, idx_t mod)
{
    if (mod == 1) return 0;                      /* any number ≡ 0 (mod 1) */

    if (start == end) {
        return (seq_t)(start % mod);             /* no exponentiation needed */
    }

    /* Reduce the exponent modulo φ(mod) via Euler's theorem. */
    uint64_t phi_mod = phi_uint64(mod);

    /* Exponent for the next level, computed modulo φ(mod). */
    seq_t exp = tower_mod_rec(start + 1, end, (idx_t)phi_mod);

    /* If the base and modulus are not coprime we must ensure the exponent
       is at least φ(mod) to avoid the reduce‑mod‑φ effect. */
    if (gcd((seq_t)start, (seq_t)mod) != 1 && exp < (seq_t)phi_mod) {
        exp += (seq_t)phi_mod;
    }

    /* exp now fits into a 64‑bit value (it is < 2*φ(mod) ≤ 2*mod). */
    return mod_pow((seq_t)start, (uint64_t)exp, (seq_t)mod);
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                        /* sequence defined for n ≥ 2 */
    if (is_power_of_two((seq_t)n)) return (seq_t)n;
    return tower_mod_rec((idx_t)2, n, n);
}
