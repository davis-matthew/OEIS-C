/*
 * A032703  Numbers k such that k prefixed by '3' and followed by '1' is
 *          prime.
 * https://oeis.org/A032703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 977, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9809 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that the decimal concatenation 3k1 is prime.
   Computed by scanning k in increasing order and testing each candidate. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/num_digits.h"
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

static seq_t *cache = NULL;      /* stored k values that satisfy the condition */
static size_t cache_len = 0;     /* number of valid terms in cache */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static idx_t next_k = 1;         /* next k to test */

/* Return the nth term (1‑based) of A032703. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;               /* undefined for 0, fall back to 0 */
    }

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        idx_t k = next_k++;
        seq_t k_seq = (seq_t)k;

        /* Number of decimal digits in k. */
        int d = num_digits(k_seq);

        /* Compute 10^(d+1) in a 64‑bit intermediate to avoid overflow of seq_t. */
        uint64_t pow10 = 1;
        for (uint64_t i = 0; i < (uint64_t)(d + 1); ++i) {
            pow10 *= 10ULL;
        }

        /* Construct N = 3 * 10^(d+1) + k * 10 + 1. */
        uint64_t N = 3ULL * pow10 + (uint64_t)k * 10ULL + 1ULL;
        seq_t term = (seq_t)N;

        /* Test primality of the constructed number. */
        if (is_prime_big(term)) {
            /* Expand the cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) {
                    abort(); /* allocation failed */
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k_seq;
        }
    }

    return cache[n - 1];
}
