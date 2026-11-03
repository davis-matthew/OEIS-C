/*
 * A142624  Primes congruent to 32 mod 55.
 * https://oeis.org/A142624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 162, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 741 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime numbers congruent to 32 modulo 55.
   a(n) is the n-th prime p such that p % 55 == 32, for n ≥ 1. */
#include <stdbool.h>
#include <stdlib.h>
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

/* Static cache of found terms, growing as needed. */
static seq_t *prime_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* The next candidate number that is ≡ 32 (mod 55). */
static seq_t next_candidate = 32;

/* Double the cache capacity. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
    if (!tmp)
        abort();          /* allocation failed */
    prime_cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th prime ≡ 32 (mod 55). */
seq_t a(idx_t n)
{
    if (n == 0)           /* offset is 1; return 0 for safety */
        return 0;

    /* Generate candidates until we have at least n terms. */
    while (cache_len < n) {
        if (is_prime_big(next_candidate)) {
            if (cache_len == cache_cap)
                grow_cache();
            prime_cache[cache_len++] = next_candidate;
        }
        /* Advance to the next 32‑mod‑55 candidate. */
        next_candidate += (seq_t)55;
    }

    return prime_cache[n - 1];
}
