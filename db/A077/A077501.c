/*
 * A077501  a(n) = smallest prime greater than a(n-1) and beginning with n.
 * https://oeis.org/A077501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest prime > a(n-1) that starts with decimal digits
 *            of n
 * Range:     u8 n <= 19, u16 n <= 654, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 63 terms through n = 4876 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses simple incremental search and memoisation.
   Because the prime with a given prefix is usually close to the prefix,
   the search space for each n is small.  The code works for both
   fixed‑size and arbitrary‑precision builds. */

#include <stdbool.h>
#include <stddef.h>

/* Utilities required by the implementation */
#include <stdint.h>
#include <stdlib.h>
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

static seq_t *cache = NULL;
static idx_t cache_cap = 0;

/* Find the smallest prime > prev that starts with decimal prefix 'pref' */
static seq_t next_prime_with_prefix(idx_t pref, seq_t prev)
{
    /* Search increasing lengths of the number of digits appended after the prefix */
    for (idx_t len = 0; /* empty */; ++len) {
        seq_t mult   = (seq_t)pref * pow10(len);          /* pref * 10^len           */
        seq_t block  = pow10(len) - (seq_t)1;             /* 10^len - 1               */
        seq_t block_max = mult + block;                   /* maximum number with this prefix */

        if (prev < block_max) {                           /* there is a candidate in this block */
            /* first candidate in this block that is > prev */
            seq_t start = mult;
            if (prev >= mult)
                start = prev + (seq_t)1;                  /* ensure strictly greater */

            /* iterate through the block to find a prime */
            for (seq_t cand = start; cand <= block_max; cand = cand + (seq_t)1) {
                if (is_prime_big(cand))
                    return cand;
            }
        }
        /* No prime in this block, try a longer representation */
    }
    /* Theoretically unreachable: by Dirichlet's theorem there will be a prime
       for some len. */
    return (seq_t)0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined for offset 0 */

    /* Ensure cache can hold indices up to n */
    if (n > cache_cap) {
        ensure_cache(n, &cache, &cache_cap);   /* may allocate more, but at least n+1 size */
        /* initialise new entries to zero */
        for (idx_t i = cache_cap; i <= n; ++i) cache[i] = (seq_t)0;
        cache_cap = n;
    }

    /* Compute all missing terms up to n (memoised) */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != (seq_t)0) continue; /* already computed */

        seq_t prev = (i > 1) ? cache[i - 1] : (seq_t)0;
        cache[i] = next_prime_with_prefix(i, prev);
    }

    return cache[n];
}
