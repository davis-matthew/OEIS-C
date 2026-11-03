/*
 * A156829  a(1) = 2. a(n) = the smallest prime >= a(n-1) + 6.
 * https://oeis.org/A156829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A156829
    a(1) = 2
    a(n) = the smallest prime ≥ a(n-1) + 6   (n > 1)
    The implementation caches all computed terms; after the
    necessary values have been generated every call is O(1).
    It works with any seq_t / idx_t implementation supplied by the
    build and is fully compliant with the C11/C++17 requirements.
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

seq_t a(idx_t n)
{
    /* static cache for the sequence values */
    static seq_t *cache   = NULL;   /* 1‑indexed array: cache[0] = a(1) */
    static idx_t cache_len = 0;     /* number of elements stored  */
    static idx_t cache_cap = 0;     /* allocated capacity          */

    if (n == 0) {                   /* undefined for 0, but return 0 */
        return 0;
    }

    /* first call: initialise the cache with a(1) */
    if (cache == NULL) {
        cache_cap = 1;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return 0;       /* allocation failure */
        cache[0] = (seq_t)2;        /* a(1) = 2 */
        cache_len = 1;
    }

    /* grow the cache if the requested index is outside the current
       stored range.  Double the capacity each time until it is large
       enough.  realloc is safe because we never shrink the allocation. */
    if (n > cache_len) {
        idx_t new_cap = cache_cap;
        while (new_cap < n) {
            new_cap *= 2;
        }
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) return 0;          /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;

        /* compute all missing terms up to n */
        for (idx_t i = cache_len; i < n; ++i) {
            /* start searching at a(i-1) + 6 */
            seq_t cand = cache[i-1] + (seq_t)6;
            while (!is_prime_big(cand)) {
                cand += (seq_t)1;
            }
            cache[i] = cand;
        }
        cache_len = n;
    }

    /* sequence is 1‑indexed: a(n) is stored at cache[n-1] */
    return cache[n-1];
}
