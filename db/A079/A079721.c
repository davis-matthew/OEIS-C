/*
 * A079721  Interemirp sequence: average of emirp pairs, analogous to
 *          interprimes.
 * https://oeis.org/A079721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (A006567(n) + A006567(n+1))/2
 * Range:     u8 n <= 14, u16 n <= 979, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A006567(n) is the n‑th emirp (prime whose reversal is a distinct prime) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* modular multiplication using 128‑bit intermediate */
static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

/* modular exponentiation */
static uint64_t pow_mod(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)
            res = mul_mod(res, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return res;
}

/* Deterministic Miller–Rabin for 64‑bit integers. */
static bool isPrime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes */
    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,
        29ULL,31ULL,37ULL,41ULL,43ULL,47ULL,53ULL,59ULL,61ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    /* write n-1 as d * 2^s */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* deterministic bases for 64 bit */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* reverse digits of a 64‑bit integer */
static uint64_t rev_uint64(uint64_t n)
{
    uint64_t rev = 0;
    while (n > 0) {
        rev = rev * 10 + (n % 10);
        n /= 10;
    }
    return rev;
}

/* dynamic array of emirps ------------------------------------------- */
static uint64_t *emirps_vec = NULL;
static size_t emirps_len = 0;
static size_t emirps_cap = 0;
static uint64_t candidate = 12;   /* next number to test (start at 13) */

static void ensure_emirps_up_to(idx_t n)
{
    while (emirps_len <= (size_t)n) {   /* need at least n+1 emirps (0‑based) */
        /* search next emirp */
        while (true) {
            ++candidate;
            if (!isPrime_uint64(candidate))
                continue;
            uint64_t revc = rev_uint64(candidate);
            if (revc != candidate && isPrime_uint64(revc))
                break;
        }
        /* store */
        if (emirps_len == emirps_cap) {
            size_t new_cap = emirps_cap ? emirps_cap * 2 : 16;
            emirps_vec = (uint64_t *)realloc(emirps_vec, new_cap * sizeof(uint64_t));
            emirps_cap = new_cap;
        }
        emirps_vec[emirps_len++] = candidate;
    }
}

seq_t a(idx_t n)
{
    /* a(1) corresponds to first two emirps (indices 0 and 1)          */
    ensure_emirps_up_to(n);   /* need emirps[n-1] and emirps[n]           */
    seq_t e1 = (seq_t)emirps_vec[(size_t)(n - 1)];
    seq_t e2 = (seq_t)emirps_vec[(size_t)(n)];
    return (e1 + e2) / 2;
}
