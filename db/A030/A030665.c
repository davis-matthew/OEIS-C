/*
 * A030665  Smallest nontrivial extension of n which is prime.
 * https://oeis.org/A030665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(n) = min{ n*10^k + d | k>=1, 0<=d<10^k, prime }
 * Range:     u8 n <= 19, u16 n <= 661, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9161 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* cache of computed values (1‑based index) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (n == 0) return 0;   /* offset is 1 */

    /* enlarge cache if needed */
    if (cache == NULL || n > cache_cap) {
        cache = seq_realloc(cache, cache_cap, n);
        /* initialize new slots */
        for (size_t i = cache_cap; i < n; ++i) cache[i] = 0;
        cache_cap = n;
    }

    /* return memoised result if present */
    if (cache[n-1] != 0) return cache[n-1];

    /* search for the smallest prime extension */
    seq_t result = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t pow10k = pow10(k);        /* 10^k as seq_t */
        seq_t base   = (seq_t)n * pow10k;  /* n * 10^k */

        /* iterate over all d in [0, 10^k-1] */
        /* pow10k fits in 64 bits for all practical k needed */
        uint64_t limit = (uint64_t)pow10k;
        for (uint64_t d = 0; d < limit; ++d) {
            seq_t candidate = base + (seq_t)d;
            if (is_prime_big(candidate)) {
                result = candidate;
                goto found;
            }
        }
    }

found:
    cache[n-1] = result;
    return result;
}
