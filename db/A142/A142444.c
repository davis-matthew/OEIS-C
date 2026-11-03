/*
 * A142444  Primes congruent to 36 mod 49.
 * https://oeis.org/A142444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 156, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 903 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142444: primes congruent to 36 (mod 49) */

/* Uses a lazy, incremental primality test (is_prime_big) and a
 * dynamic cache to store found primes.  No sieve is needed.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

/* Cache of found primes ≡ 36 (mod 49).  */
static seq_t *cache      = NULL;   /* array of primes */
static size_t cache_len  = 0;      /* number of stored primes */
static size_t cache_cap  = 0;      /* current capacity of cache */

/* Next number of the form 49*k+36 to test for primality.  */
static idx_t next_candidate = 36;

/* Grow the cache if necessary to accommodate at least one more element. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();
    cache          = tmp;
    cache_cap      = new_cap;
}

/* Ensure that the cache holds at least n primes. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n) return;

    /* Prepare cache if it's the first call. */
    if (!cache) grow_cache();

    while (cache_len < n) {
        /* Search for the next prime of the required form. */
        while (!is_prime_big((seq_t)next_candidate))
            next_candidate += 49;

        /* Store it. */
        if (cache_len + 1 > cache_cap) grow_cache();
        cache[cache_len++] = (seq_t)next_candidate;

        /* Prepare the candidate for the next search. */
        next_candidate += 49;
    }
}

/* OEIS sequence access function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset 0 is undefined */
    ensure_cache(n);
    return cache[n - 1];
}
