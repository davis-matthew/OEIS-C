/*
 * A159559  Lexicographically first strictly increasing sequence starting a(2)
 *          = 3 with the property that a(n) is prime if and only if n is
 *          prime.
 * https://oeis.org/A159559
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m > a(n-1) such that m is prime iff n is prime,
 *            with a(2) = 3
 * Range:     u8 n <= 150, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9412 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
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

/* Cache of computed sequence values */
static seq_t *seq_cache = NULL;
static idx_t cache_cap = 0;   /* allocated capacity (size_t indices) */
static idx_t cache_size = 0;  /* highest index that has been computed */

/* Grow the cache so that index 'idx' is valid for write */
static void ensure_cache(idx_t idx)
{
    if (idx < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap <= idx)
        new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
    seq_cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;  /* not defined for n < 2 */

    ensure_cache(n);

    /* Initialise the very first value a(2)=3 if not already done */
    if (cache_size < 2) {
        seq_cache[2] = (seq_t)3;
        cache_size = 2;
    }

    for (idx_t k = cache_size + 1; k <= n; ++k) {
        seq_t prev = seq_cache[k - 1];
        seq_t cur  = prev + (seq_t)1;

        if (is_prime(k)) {        /* need a prime */
            while (!is_prime_big(cur))
                ++cur;
        } else {                  /* need a composite */
            while (is_prime_big(cur))
                ++cur;
        }

        seq_cache[k] = cur;
    }

    cache_size = n;
    return seq_cache[n];
}
