/*
 * A163578  If n is prime, a(n) is next composite greater than a(n-1), else if
 *          n is composite, a(n) is next prime greater than a(n-1) else 2.
 * https://oeis.org/A163578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 7489, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9975 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A163578:
 * a(1)=2; for n>1:
 *   if n is prime → a(n) is the smallest composite > a(n‑1)
 *   if n is composite (including 1) → a(n) is the smallest prime > a(n‑1)
 * (n=1 is treated as composite). */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"
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

static seq_t *cache      = NULL;   /* memoised terms, a(1) at cache[0] */
static size_t cache_size = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity */

/* Ensure the cache can hold at least new_cap terms. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t new_cap_real = new_cap > cache_cap * 2 ? new_cap : cache_cap * 2;
    if (new_cap_real == 0) new_cap_real = new_cap;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap_real);
    if (!tmp && new_cap_real > 0) exit(EXIT_FAILURE); /* allocation failed */
    cache = tmp;
    cache_cap = new_cap_real;
}

/* Return the first prime number strictly greater than x. */
static seq_t next_prime(seq_t x)
{
    seq_t y = x + 1;
    while (!is_prime_big(y)) ++y;
    return y;
}

/* Return the first composite number strictly greater than x (x>0). */
static seq_t next_composite(seq_t x)
{
    seq_t y = x + 1;
    while (y <= (seq_t)1 || is_prime_big(y)) ++y;
    return y;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined input, harmless return */

    /* initialise cache lazily */
    if (!cache) {
        cache_cap  = 64;
        cache      = mem_new(seq_t, cache_cap);
        cache_size = 0;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t prev = (i == 1) ? (seq_t)0 : cache[i - 2];
        seq_t val;
        if (is_prime(i)) {            /* n is prime */
            val = next_composite(prev);
        } else {                       /* n is composite (or 1) */
            val = next_prime(prev);
        }
        grow_cache(cache_size + 1);
        cache[cache_size++] = val;
    }

    return cache[n - 1];
}
