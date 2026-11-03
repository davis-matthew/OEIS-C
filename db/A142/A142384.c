/*
 * A142384  Primes congruent to 33 mod 47.
 * https://oeis.org/A142384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 140, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 868 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142384: primes congruent to 33 (mod 47)
   Computation method: iterate candidates of the form 47*k+33,
   test each for primality with is_prime_big(), and cache the
   results so subsequent calls for larger n do not redo work. */

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

static seq_t *prime_cache = NULL;
static size_t cache_cap  = 0;
static size_t cache_cnt  = 0;
static idx_t  next_k      = 2;     /* 47*2+33 = 127 is the first prime */

/* Ensure the cache has at least n primes. */
static void ensure_cached(idx_t n)
{
    if (cache_cnt >= (size_t)n)
        return;                                  /* already enough */

    /* Allocate an initial cache if uninitialized. */
    if (cache_cap == 0) {
        cache_cap = 32;
        prime_cache = mem_new(seq_t, cache_cap);
    }

    while (cache_cnt < (size_t)n) {
        /* Generate next candidate in the residue class 33 (mod 47). */
        seq_t candidate = (seq_t)47 * (seq_t)next_k + (seq_t)33;
        ++next_k;

        /* Test for primality. */
        if (is_prime_big(candidate)) {
            /* Grow cache if necessary. */
            if (cache_cnt == cache_cap) {
                cache_cap *= 2;
                prime_cache = mem_resize(seq_t, prime_cache, cache_cap);
            }
            prime_cache[cache_cnt++] = candidate;
        }
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* sequence is undefined for offset < 1 */
    ensure_cached(n);                /* make sure the n‑th element is cached */
    return prime_cache[(size_t)(n - 1)];
}
