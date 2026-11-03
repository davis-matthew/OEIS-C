/*
 * A138626  Nonnegative integers n such that 17*n-5 is prime.
 * https://oeis.org/A138626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 578, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 942 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th nonnegative integer k such that 17*k‑5 is prime (OEIS A138626) */

#include <stdbool.h>
#include <stddef.h>
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

/* static cache of discovered terms */
static seq_t *cache = NULL;
static size_t cache_size   = 0;   /* number of terms stored */
static size_t cache_cap    = 0;   /* allocated capacity */
static idx_t  test_candidate = 2; /* next candidate value to test */

/* ensure the cache has room for at least 'need' elements */
static void
ensure_cache_cap(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t
a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1, but return 0 */

    /* generate terms until we have at least 'n' of them */
    while (cache_size < (size_t)n) {
        /* compute 17 * test_candidate – 5 in a type wide enough to avoid
           overflow for the expected ranges */
        unsigned __int128 val128 = (unsigned __int128)17 * test_candidate;
        val128 -= 5;
        seq_t val = (seq_t)val128;  /* truncation is safe for the expected range */

        if (is_prime_big(val)) {
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = test_candidate;
        }
        ++test_candidate;
    }

    return cache[(size_t)n - 1];
}
