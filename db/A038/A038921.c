/*
 * A038921  Primes p such that 42 is a square mod p.
 * https://oeis.org/A038921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A038921: primes p for which 42 is a quadratic residue modulo p.
 *  A good prime p satisfies (42|p) = 1.
 *
 *  The implementation computes the n‑th good prime on demand.
 *  It uses a simple incremental prime generator with trial division
 *  and Legendre’s criterion via modular exponentiation.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/*  Auxiliary: modular exponentiation  (base^exp mod mod)
 *  works for 64‑bit primes – all built‑in utilities are avoided.
 */
static inline seq_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    unsigned __int128 result = 1;
    unsigned __int128 b = base % mod;
    while (exp) {
        if (exp & 1U) result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1U;
    }
    return (seq_t)result;
}

/*  Storage for all small primes found so far – needed for primality test. */
static uint64_t *small_primes   = NULL;
static size_t   small_cnt       = 0;
static size_t   small_cap       = 0;
static uint64_t last_checked    = 0;   /* last integer examined */

/*  Storage for good primes (answers of the sequence). */
static seq_t *good_primes      = NULL;
static idx_t  good_cnt         = 0;
static idx_t  good_cap         = 0;

/*  Ensure capacity for prime list. */
static void ensure_small_cap(size_t need)
{
    if (small_cap < need) {
        size_t newcap = small_cap ? small_cap * 2 : 16;
        if (newcap < need) newcap = need;
        uint64_t *tmp = (uint64_t *)realloc(small_primes, newcap * sizeof(uint64_t));
        if (!tmp) abort();
        small_primes = tmp;
        small_cap    = newcap;
    }
}

/*  Ensure capacity for good prime list. */
static void ensure_good_cap(size_t need)
{
    if (good_cap < need) {
        size_t newcap = good_cap ? good_cap * 2 : 16;
        if (newcap < need) newcap = need;
        seq_t *tmp = (seq_t *)realloc(good_primes, newcap * sizeof(seq_t));
        if (!tmp) abort();
        good_primes = tmp;
        good_cap    = newcap;
    }
}

/*  Test whether candidate is prime using the primes discovered so far. */
static bool is_prime_candidate(uint64_t candidate)
{
    for (size_t i = 0; i < small_cnt; ++i) {
        uint64_t p = small_primes[i];
        if (p * p > candidate) break;
        if (candidate % p == 0) return false;
    }
    return true;
}

/*  Determine whether p is a good prime (42 a quadratic residue mod p). */
static bool is_good_prime(uint64_t p)
{
    if (p == 2 || p == 3 || p == 7) return true;   /* 42 ≡ 0 (mod p)   */
    return mod_pow_uint64(42ULL, (p - 1) / 2, p) == 1;
}

/*  Expand the good prime cache until it contains at least n elements. */
static void ensure_count(idx_t n)
{
    while (good_cnt < n) {
        /* Find next prime after last_checked. */
        uint64_t candidate = last_checked + 1;
        if (candidate <= 2) candidate = 2;
        while (!is_prime_candidate(candidate)) ++candidate;

        /* Store the discovered prime. */
        ensure_small_cap(small_cnt + 1);
        small_primes[small_cnt++] = candidate;
        last_checked = candidate;

        /* Check if it satisfies the Legendre condition. */
        if (is_good_prime(candidate)) {
            ensure_good_cap(good_cnt + 1);
            good_primes[good_cnt++] = (seq_t)candidate;
        }
    }
}

/*  Public sequence routine.  Assumes 1‑based indexing. */
seq_t a(idx_t n)
{
    if (n == 0) abort();           /* offset is 1 – caller must use n ≥ 1 */
    ensure_count(n);
    return good_primes[n - 1];
}
