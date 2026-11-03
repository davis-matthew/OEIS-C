/*
 * A023252  Primes that remain prime through 2 iterations of function f(x) =
 *          5x + 2.
 * https://oeis.org/A023252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that 5p+2 and 25p+12 are also
 *            primes
 * Range:     u8 none, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 929 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

seq_t a(idx_t n) {
    /* A static cache that holds all terms already found. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* The next candidate prime to test. */
    static seq_t cur = 2;

    /* Initialise the cache the first time the function is called. */
    if (cache == NULL) {
        cache_cap = 128;
        cache = mem_new(seq_t, cache_cap);
    }

    /* Generate additional terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Find the next prime candidate. */
        do {
            cur++;
            /* Skip even numbers after the first prime 2. */
            if ((cur & 1) == 0)
                continue;
        } while (!is_prime_big(cur));

        seq_t p = cur;
        seq_t q = p * (seq_t)5 + (seq_t)2;      /* 5p + 2 */
        seq_t r = q * (seq_t)5 + (seq_t)2;      /* 25p + 12 */

        /* If both derived numbers are prime, store p. */
        if (is_prime_big(q) && is_prime_big(r)) {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = p;
        }
    }

    return cache[n - 1];
}
