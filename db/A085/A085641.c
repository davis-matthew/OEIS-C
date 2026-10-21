/*
 * A085641  Smallest prime == 1 (mod p*q*...*k) where p, q, ..., k are all the
 *          distinct prime divisors of n. Or, smallest prime == 1 (mod the
 *          largest squarefree divisor of n).
 * https://oeis.org/A085641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest prime p such that p ≡ 1 (mod rad(n)), where
 *            rad(n) is the product of the distinct prime factors of n
 * Range:     u8 n <= 30, u16 n <= 1690, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mod_mul.h"

/* ---------- deterministic Miller–Rabin primality test for 64‑bit integers ---------- */

static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes trial division */
    static const uint64_t small_primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
    };
    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    /* Miller–Rabin with deterministic bases for 64‑bit numbers */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* known sufficient set of bases for 64‑bit integers */
    static const uint64_t bases[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
    };
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;             /* safety for index 0 */

    uint64_t nn = (uint64_t)n;
    uint64_t rad = 1;                         /* rad(n): product of distinct primes */

    /* factorise nn to obtain rad */
    uint64_t temp = nn;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            rad *= p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) rad *= temp;               /* final prime factor */

    /* find the smallest prime ≡ 1 (mod rad) */
    if (rad == 1) return (seq_t)2;            /* rad(1)=1 → first prime 2 */

    uint64_t candidate = rad + 1;
    while (true) {
        if (is_prime_uint64(candidate)) return (seq_t)candidate;
        if (candidate > (UINT64_MAX - rad)) break; /* protect against overflow */
        candidate += rad;
    }
    /* fallback – should never happen for the tested range */
    return (seq_t)0;
}
