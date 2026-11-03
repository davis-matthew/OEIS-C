/*
 * A165350  Primes p such that floor((p^2-1)/4)+p is not prime.
 * https://oeis.org/A165350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Prime p such that floor((p^2-1)/4)+p is not prime (composite)
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 990 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function iteratively builds the sequence by testing primes in order
   and caching those that satisfy the composite condition.  It works for
   any seq_t/idx_t pair provided the helper is_prime_big is available. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* ----  External utilities ---- */

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ----  Cache of qualifying primes ---- */
static seq_t *qual_cache = NULL; /* array of primes that satisfy the condition */
static size_t cache_len = 0;     /* number of cached terms */
static size_t cache_cap = 0;     /* current capacity of qual_cache */

/* ----  Cache of all primes (used for primality testing of candidates) ---- */
static seq_t *prime_list = NULL; /* array of all primes found so far */
static size_t primes_len = 0;    /* number of primes in prime_list */
static size_t primes_cap = 0;    /* capacity of prime_list */

/* ----  Grow a seq_t array to at least 'need' elements. ---- */
static void ensure_cap(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need)
        return;
    size_t new_cap = *cap ? *cap << 1 : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();        /* simple error handling */
    *arr = tmp;
    *cap = new_cap;
}

/* ----  Integer square root (floor) for seq_t values. ---- */
static seq_t isqrt_seq(seq_t n)
{
    seq_t x = 0;
    while ((x + 1) * (x + 1) <= n)
        ++x;
    return x;
}

/* ----  Test whether n is prime using trial division by previously
   generated primes.  It falls back to is_prime_big for very large
   candidates that exceed the square of the largest cached prime. ---- */
static bool is_prime_candidate(seq_t n)
{
    if (n < 2)
        return false;
    seq_t root = isqrt_seq(n);
    for (size_t i = 0; i < primes_len; ++i) {
        seq_t p = prime_list[i];
        if (p > root)
            break;
        if (n % p == 0)
            return false;
    }
    /* All trials passed: determine primality using the general test. */
    return is_prime_big(n);
}

/* ----  Generate the next prime greater than the last returned by
   this generator, using the prime_list for test division. ---- */
static seq_t next_prime(void)
{
    static seq_t candidate = 2;
    static bool first = true;

    if (first) {
        first = false;
        /* Return 2 on the first call, then start from 3. */
        candidate = 3;
        /* Store 2 in prime_list. */
        ensure_cap(&prime_list, &primes_cap, primes_len + 1);
        prime_list[primes_len++] = 2;
        return 2;
    }

    while (!is_prime_candidate(candidate)) {
        candidate += (candidate == 2) ? 1 : 2; /* skip even numbers */
    }
    /* candidate is prime */
    seq_t prime = candidate;
    candidate += 2; /* next odd candidate */

    /* Cache this prime for future trial divisions */
    ensure_cap(&prime_list, &primes_cap, primes_len + 1);
    prime_list[primes_len++] = prime;

    return prime;
}

/* ----  Main sequence function a(idx_t n) ---- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* safety value for offsets < 1 */

    /* Expand cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t p = next_prime();

        /* Compute floor((p^2-1)/4)+p */
        seq_t val = p * p - 1;
        val = val / 4;
        val += p;

        if (!is_prime_big(val)) {      /* composite ⇒ qualifying prime */
            ensure_cap(&qual_cache, &cache_cap, cache_len + 1);
            qual_cache[cache_len++] = p;
        }
    }

    return qual_cache[n - 1];
}
