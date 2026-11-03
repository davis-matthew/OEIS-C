/*
 * A175442  a(n)>a(n-1), a(n) = smallest prime such that a(n)+a(n-1) is
 *          multiple of m, a(1)=2, m=3.
 * https://oeis.org/A175442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(1)=2; for n>1 a(n) = smallest prime > a(n-1)
   such that (a(n)+a(n-1)) % 3 == 0 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

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

static seq_t *cache = NULL;       /* cache[0] = a(1) */
static size_t cache_len = 0, cache_cap = 0;

/* Grow the cache so that it has at least `new_cap` elements. */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Return the nth term (n>=1). */
seq_t a(idx_t n)
{
    if (n == 0)               /* behaviour for n<1 is irrelevant */
        return (seq_t)0;

    /* Initialise cache on first call. */
    if (!cache) {
        grow_cache();
        cache[0] = (seq_t)2;   /* a(1) */
        cache_len = 1;
    }

    /* Extend cache up to n. */
    while (cache_len < (size_t)n) {
        seq_t const prev = cache[cache_len - 1];   /* a(k-1), where k = cache_len+1 */

        /* Find the remainder that a(k) must have to make the sum divisible by 3. */
        unsigned int prev_mod   = (unsigned int)(prev % (seq_t)3);          /* 0,1,2 */
        unsigned int target_mod = (3 - prev_mod) % 3;                      /* 0,1,2 */

        /* Candidate just greater than prev with correct remainder. */
        seq_t cand = prev + (seq_t)1;
        unsigned int cand_mod = (unsigned int)(cand % (seq_t)3);
        unsigned int shift = (target_mod + 3 - cand_mod) % 3;
        if (shift != 0)
            cand += (seq_t)shift;

        /* Search for the next prime with the required remainder. */
        while (!is_prime_big(cand)) {
            cand += (seq_t)3;   /* keep the same modulo 3 remainder */
        }

        /* Store the new term, growing the array if necessary. */
        if (cache_len == cache_cap)
            grow_cache();
        cache[cache_len++] = cand;
    }

    return cache[n - 1];
}
