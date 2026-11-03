/*
 * A221211  Numbers n such that n and n + 4 are prime and there is a power of
 *          two in the interval (n,n+4).
 * https://oeis.org/A221211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A221211: numbers n such that n and n+4 are prime and a power of two lies in (n,n+4). */
/* The function searches over powers of two, testing whether t-1, t-2 or t-3
   are primes with the corresponding t+3, t+2, or t+1 also prime, and
   collects the qualifying n in a memoised array. */

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

seq_t a(idx_t n)
{
    /* Static memoisation state */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t cur_pow = 0;
    static bool initialized = false;

    /* Initialise on first call */
    if (!initialized) {
        cur_pow = (seq_t)4;      /* 2^2, the first relevant power of two */
        initialized = true;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        /* For each power of two t = cur_pow check t-1, t-2, t-3 */
        for (int d = 1; d <= 3; ++d) {
            seq_t p = cur_pow - (seq_t)d;
            if (p < (seq_t)2) continue;
            if (is_prime_big(p) && is_prime_big(p + (seq_t)4)) {
                if (cache_len == cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                    if (!tmp) return 0;  /* Out of memory – unlikely in tests */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = p;
                if (cache_len >= n) break;  /* stop adding more for now */
            }
        }
        cur_pow = cur_pow * (seq_t)2;
    }

    return cache[n - 1];
}
