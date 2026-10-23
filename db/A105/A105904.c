/*
 * A105904  Primes for which -32 is a primitive root.
 * https://oeis.org/A105904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   modmul_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the n‑th prime p for which -32 is a primitive root modulo p.
   The computation is performed by a direct search: 
   1. Iterate over odd numbers >2, test primality.
   2. For each prime p, test whether -32 is a primitive root mod p by checking that
      (-32)^{(p−1)/q} ≠ 1 (mod p) for every distinct prime divisor q of p−1.
   3. Return the n‑th such prime. The result is cast to seq_t, which may be an
      arbitrary‑precision type in the build. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/modmul_uint64.h"

/* modular exponentiation: base^exp mod mod */
static uint64_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = modmul_uint64(result, base, mod);
        base = modmul_uint64(base, base, mod);
        exp >>= 1ULL;
    }
    return result;
}

/* simple deterministic primality test for 64‑bit integers.
   Works fast for the small numbers needed here (first ~10 000 primes). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)            return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1ULL) == 0)  return false;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* test if -32 is a primitive root modulo prime p */
static bool is_primitive_minus32(uint64_t p)
{
    if (p == 2) return false;          /* -32 mod 2 = 0, not a primitive root */
    uint64_t phi = p - 1;              /* Euler's totient for prime p */

    /* (-32) mod p is simply p - (32 % p)  (unless 32%p==0, then 0) */
    uint64_t r = 32 % p;
    uint64_t base = (r == 0) ? 0 : (p - r);

    /* factor phi into distinct prime factors */
    uint64_t tmp = phi;
    uint64_t factors[64];
    size_t nf = 0;

    if (tmp % 2ULL == 0ULL) {
        factors[nf++] = 2ULL;
        while (tmp % 2ULL == 0ULL) tmp /= 2ULL;
    }
    for (uint64_t d = 3ULL; d * d <= tmp; d += 2ULL) {
        if (tmp % d == 0ULL) {
            factors[nf++] = d;
            while (tmp % d == 0ULL) tmp /= d;
        }
    }
    if (tmp > 1ULL) factors[nf++] = tmp;

    /* Check the primitive‑root condition */
    for (size_t i = 0; i < nf; ++i) {
        uint64_t q = factors[i];
        if (modpow_uint64(base, phi / q, p) == 1ULL)
            return false;               /* not a primitive root */
    }
    return true;
}

seq_t a(idx_t n)
{
    /* n is 1‑based */
    uint64_t count   = 0;
    uint64_t candidate = 3ULL;   /* start at the first odd >2 */

    while (1) {
        if (is_prime_uint64(candidate) && is_primitive_minus32(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        candidate += 2ULL;          /* test next odd number */
    }
}
