/*
 * A104087  Smallest prime >= 11^n.
 * https://oeis.org/A104087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   isprime_big, mod_mul_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A104087: Smallest prime ≥ 11ⁿ
   Formula: a(n) = min { p prime | p ≥ 11ⁿ }
   The implementation works for all seq_t sizes.  For seq_t that
   fit in 8 bytes we use an efficient deterministic Miller–Rabin
   test for 64‑bit integers; for larger seq_t we defer to
   the provided big‑integer primality routine isprime_big.
   The power 11ⁿ is computed by simple repeated multiplication,
   which is safe for seq_t up to 10000 indices (11¹⁰⁰⁰⁰ ≈ 10²⁸⁵⁸).
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isprime_big.h"
#include "../utils/mod_mul_uint64.h"

/* deterministic Miller–Rabin for 64‑bit integers.
   Bases 2,3,5,7,11,13,17 are sufficient for all n < 2⁶⁴. */

static uint64_t mod_pow_uint64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    while (e) {
        if (e & 1) res = mod_mul_uint64(res, a, mod);
        a = mod_mul_uint64(a, a, mod);
        e >>= 1;
    }
    return res;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(*small_primes); ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(small_primes)/sizeof(*small_primes); ++i) {
        uint64_t a = small_primes[i];
        if (a >= n) break;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul_uint64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Return true if candidate of type seq_t is prime.
   For seq_t that fits in 64 bits we use the fast test above,
   otherwise we delegate to isprime_big. */
static bool is_prime_seq(seq_t candidate)
{
    if (sizeof(seq_t) <= sizeof(uint64_t))
        return is_prime_uint64((uint64_t)candidate);
    else
        return isprime_big(candidate);
}

seq_t a(idx_t n)
{
    /* 1. compute 11^n */
    seq_t power = 1;
    const seq_t base = 11;
    for (idx_t i = 0; i < n; ++i) power *= base;

    /* 2. find smallest prime >= 11^n */
    seq_t candidate;
    if (power < 2)            /* n = 0 → 1, so start at 2 */
        candidate = 2;
    else if ((power & 1) == 0)    /* make odd if necessary */
        candidate = power + 1;
    else
        candidate = power;

    while (!is_prime_seq(candidate))
        candidate += 2;       /* test only odd numbers */

    return candidate;
}
