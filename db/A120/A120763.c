/*
 * A120763  a(1) = 2. a(n) = a(n-1)*(largest prime occurring earlier in the
 *          sequence) - 1.
 * https://oeis.org/A120763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; for n>1, a(n) = a(n-1) * (largest prime occurring in
 *            a(1)..a(n-1)) – 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-03)
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

/* 1‑based cache for previously computed terms */
static seq_t *cache = NULL;          /* stores a(i) at cache[i] */
static idx_t  cache_cap = 0;         /* allocated size of cache array */
static idx_t  cached_up_to = 0;      /* largest index computed so far */
static seq_t  largest_prime = (seq_t)0;   /* largest prime seen so far */

seq_t a(idx_t n)
{
    /* lazy initialisation */
    if (cache == NULL) {
        /* reserve space for indices 0 and 1 (index 0 unused) */
        cache_cap = 2;
        cache = mem_new(seq_t, cache_cap);
        /* a(1) = 2 */
        cache[1] = (seq_t)2;
        largest_prime = (seq_t)2;     /* 2 is the first prime */
        cached_up_to = 1;
    }

    /* extend the cache if a(n) is not yet computed */
    if (n > cached_up_to) {
        /* grow the array until its capacity exceeds n */
        if (cache_cap <= n) {
            size_t new_cap = cache_cap;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            cache = tmp;
            cache_cap = (idx_t)new_cap;
        }

        /* compute terms up to n */
        for (idx_t i = cached_up_to + 1; i <= n; ++i) {
            seq_t val = cache[i - 1] * largest_prime - (seq_t)1;
            cache[i] = val;

            /* update the largest prime if val is prime */
            if (is_prime_big(val) && val > largest_prime) {
                largest_prime = val;
            }
        }
        cached_up_to = n;
    }

    return cache[n];
}
