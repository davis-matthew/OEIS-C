/*
 * A142388  Primes congruent to 37 mod 47.
 * https://oeis.org/A142388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 150, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 959 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime congruent to 37 (mod 47).
   The sequence is defined by a(1)=37 and each subsequent term is
   the next prime ≡ 37 (mod 47).  No closed‑form expression is known. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
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

/* Static cache to store already found primes.
   For n up to about 10 000 the term is < 1 000 000, so this size is safe. */
#define CACHE_CAP 20000            /* must be ≥ maximum n that will be requested */

static seq_t cache[CACHE_CAP];
static size_t cache_size = 0;      /* number of primes stored */

/* Return the nth term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; undefined for n<1 */

    /* If requested term is already in the cache, return it. */
    if ((size_t)n <= cache_size) return cache[n - 1];

    /* Generate missing primes until the cache contains at least n elements. */
    seq_t cand;
    if (cache_size == 0) {
        cand = (seq_t)37;          /* first term */
    } else {
        cand = cache[cache_size - 1] + (seq_t)47;   /* next candidate in this congruence */
    }

    while (cache_size < (size_t)n) {
        /* Find the next prime in the congruence class. */
        while (!is_prime_big(cand)) cand += (seq_t)47;

        /* Store it in the cache. */
        cache[cache_size] = cand;
        cache_size++;

        /* Prepare candidate for the next iteration. */
        cand += (seq_t)47;
    }

    return cache[n - 1];
}
