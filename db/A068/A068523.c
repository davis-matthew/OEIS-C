/*
 * A068523  Essentially the same as A055500.
 * https://oeis.org/A068523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = largest prime ≤ a(n-1) + a(n-2), with a(1)=2, a(2)=3
 * Range:     i8 n <= 10, i16 n <= 20, i32 n <= 20, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
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

static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of terms currently stored */
static size_t cache_cap = 0;  /* allocated capacity */

/* ensure the cache can hold at least `needed` elements (where needed ≤ 2^64) */
static void ensure_capacity(idx_t needed)
{
    if ((idx_t)cache_cap >= needed) return;
    size_t new_cap = needed > 64 ? needed : 64;
    while ((idx_t)new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;     /* undefined for n<1 */

    if (cache_len < n) {
        ensure_capacity(n);
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i == (idx_t)1) {
                cache[0] = (seq_t)2;
            } else if (i == (idx_t)2) {
                cache[1] = (seq_t)3;
            } else {
                seq_t sum      = cache[i - 2] + cache[i - 3];
                seq_t candidate = sum;
                while (!is_prime_big(candidate)) {
                    candidate -= (seq_t)1;
                }
                cache[i - 1] = candidate;
            }
        }
        cache_len = n;
    }
    return cache[n - 1];
}
