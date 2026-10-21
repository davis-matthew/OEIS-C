/*
 * A235376  Positions of 9's in A235354.
 * https://oeis.org/A235376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   mod_mul, to_decimal_from_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A235376
 *
 *  a(n) = the nth index (in the sequence of primes) of a prime p
 *  such that when p is written in base k (2 ≤ k ≤ 8) the decimal
 *  interpretation of the base‑k digit string is composite, while
 *  for k = 9 the decimal interpretation is prime.
 *
 *  The function below generates primes lazily, checks the above
 *  property, caches all qualifying indices, and returns the n‑th
 *  one for any n ≥ 1 (indices are 1‑based exactly as in OEIS).
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_mul.h"
#include "../utils/to_decimal_from_base.h"

/* ------------------------------------------------------------------ */
/* Simple 64‑bit modular multiplication and exponentiation for primality
 * test.  These routines are used by isPrime64() and are sufficient
 * for the ranges encountered here. */

static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1)
            result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin for 64‑bit integers. */
static bool isPrime64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t small[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n == small[i])
            return true;
        if (n % small[i] == 0)
            return false;
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t witnesses[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0)
            continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Integer square root returning floor(sqrt(n)).  Uses the
 * standard bit‑wise Newton iteration which is fast, integer only. */
static uint64_t isqrt64(uint64_t n)
{
    if (n == 0 || n == 1)
        return n;
    uint64_t x = n >> 1;
    uint64_t y = (x + n / x) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Check whether prime p satisfies the A235376 property. */
static bool satisfies_property(uint64_t p)
{
    uint64_t val9 = to_decimal_from_base(p, 9);
    if (!isPrime64(val9))
        return false;
    for (int base = 2; base <= 8; ++base) {
        uint64_t val = to_decimal_from_base(p, base);
        if (isPrime64(val))
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* --- Static state for lazy prime generation and caching --- */
static uint64_t *prime_list = NULL;
static size_t prime_cnt  = 0;
static size_t prime_cap  = 0;
static uint64_t next_candidate = 3;          /* next odd number to test */

static idx_t *pos_list = NULL;          /* indices of qualifying primes */
static size_t pos_cnt  = 0;
static size_t pos_cap  = 0;

/* Ensure that the static arrays are allocated at least once. */
static void ensure_allocations(void)
{
    if (!prime_list) {
        prime_cap = 128;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }
    if (!pos_list) {
        pos_cap = 64;
        pos_list = (idx_t *)malloc(pos_cap * sizeof(idx_t));
    }
}

/* Expand prime_list to the requested capacity. */
static void grow_prime_list(void)
{
    prime_cap *= 2;
    prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
}

/* Expand pos_list to the requested capacity. */
static void grow_pos_list(void)
{
    pos_cap *= 2;
    pos_list = (idx_t *)realloc(pos_list, pos_cap * sizeof(idx_t));
}

/* Generate the next prime number; appends it to prime_list and
 * returns its value.  The prime indices are 1‑based. */
static uint64_t next_prime(void)
{
    if (prime_cnt == 0) {           /* initialise with the first prime */
        prime_list[0] = 2;
        prime_cnt = 1;
        next_candidate = 3;
        return 2;
    }

    while (1) {
        uint64_t cand = next_candidate;
        next_candidate += 2;        /* next odd number */

        uint64_t limit = isqrt64(cand);
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            if (prime_list[i] > limit)
                break;
            if (cand % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (!is_prime)
            continue;               /* skip composite */

        /* prime found */
        if (prime_cnt == prime_cap)
            grow_prime_list();
        prime_list[prime_cnt++] = cand;
        return cand;
    }
}

/* Ensure that we have computed at least need terms of the sequence. */
static void ensure_positions(idx_t need)
{
    if (need <= pos_cnt)
        return;

    ensure_allocations();

    while (pos_cnt < need) {
        uint64_t p = next_prime();   /* generates primes until we have enough */
        if (satisfies_property(p)) {
            if (pos_cnt == pos_cap)
                grow_pos_list();
            pos_list[pos_cnt++] = (idx_t)(prime_cnt);   /* prime index (1‑based) */
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n = 0; return 0 */

    ensure_positions(n);
    /* pos_list is 0‑based but a(n) is 1‑based */
    return (seq_t)pos_list[n - 1];
}
