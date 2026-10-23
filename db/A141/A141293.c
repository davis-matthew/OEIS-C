/*
 * A141293  Primes p of the form 4*k+1 which are not of the form r^2 + 1.
 * https://oeis.org/A141293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code implements OEIS sequence A141293.
   The sequence consists of primes of the form 4*k+1 that are not of
   the form r^2+1.  The function searches for the required primes by
   enumerating odd integers, testing them for primality with a
   deterministic Miller–Rabin routine for 64‑bit numbers, and
   rejecting those of the form r^2+1.  No external utilities are
   required.  The code works for all supplied integer types
   (seq_t, idx_t).  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Determines whether a 64‑bit integer is a perfect square.
   Returns floor(sqrt(n)).  */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = n, mid;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        uint64_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;   /* hi is floor(sqrt(n)) when loop exits */
}

/* modular multiplication: (a*b) % mod, safe for 64‑bit values. */
static uint64_t mod_mul_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* modular exponentiation: (base^exp) % mod. */
static uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp > 0) {
        if (exp & 1) result = mod_mul_u64(result, b, mod);
        b = mod_mul_u64(b, b, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin primality test for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)               return false;
    if (n % 2ULL == 0ULL)    return n == 2ULL;
    if (n % 3ULL == 0ULL)    return n == 3ULL;
    if (n % 5ULL == 0ULL)    return n == 5ULL;
    if (n % 7ULL == 0ULL)    return n == 7ULL;
    if (n % 11ULL == 0ULL)   return n == 11ULL;

    /* Write n-1 as d*2^s with d odd. */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    /* Bases sufficient for deterministic test for 64‑bit numbers. */
    const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = mod_pow_u64(a, d, n);
        if (x == 1ULL || x == n - 1) continue;
        bool cont = false;
        for (unsigned r = 1; r < s; ++r) {
            x = mod_mul_u64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (cont) continue;
        return false;
    }
    return true;
}

/* Main function required by the exercise. */
seq_t a(idx_t n)
{
    /* The sequence is indexed starting at 1.  n ≥ 1. */
    idx_t count = 0;
    uint64_t p = 5ULL;          /* start testing at the smallest 4k+1 prime */
    for (;;) {
        if (is_prime_uint64(p)) {
            if ((p & 3ULL) == 1ULL) {                     /* p ≡ 1 (mod 4) */
                uint64_t t = p - 1;
                uint64_t r = isqrt_u64(t);
                if (r * r + 1ULL != p) {                  /* not of form r²+1 */
                    ++count;
                    if (count == n) {
                        return (seq_t)p;
                    }
                }
            }
        }

        /* Move to the next odd integer.  Skip even numbers. */
        if (p == 5ULL)      p = 7ULL;
        else                p += 2ULL;
    }
}
