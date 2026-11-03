/*
 * A166581  Primes containing the string 333.
 * https://oeis.org/A166581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   primes that contain the decimal substring "333" (OEIS A166581)
 * Range:     u8 none, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 50 terms through n = 993 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
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

/* Utility: check whether x contains the substring 333 */
static inline bool has_333(idx_t x)
{
    unsigned cnt = 0;
    while (x > 0) {
        if (x % 10 == 3) {
            cnt++;
            if (cnt == 3) return true;
        } else {
            cnt = 0;
        }
        x /= 10;
    }
    return false;
}

/* Cache for computed terms */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;     /* number of allocated elements (indexable up to cache_cap-1) */
static idx_t  cache_max  = 0;     /* highest index that has been computed (1‑based) */

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1 – n=0 is outside the OEIS definition */

    /* Expand cache if necessary */
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
        if (!new_cache) return 0;           /* allocation failure – return 0 */
        /* Zero the newly added part to keep new_cache[0]=0 invariant */
        for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed */
    if (n <= cache_max) return cache[n];

    /* Compute missing terms up to n */
    idx_t candidate = (cache_max == 0) ? 2 : ((idx_t)cache[cache_max]) + 1;  /* start after last prime */
    while (cache_max < n) {
        if (has_333(candidate) && is_prime_big((seq_t)candidate)) {
            cache[++cache_max] = (seq_t)candidate;
        }
        ++candidate;
    }

    return cache[n];
}
