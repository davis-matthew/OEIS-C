/*
 * A178663  a(1)=1. For n>1, a(n) is the smallest number greater than a(n-1)
 *          such that exactly one of n and a(n) is prime and the other is
 *          composite.
 * https://oeis.org/A178663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(1)=1; for n>1, a(n) is the smallest number > a(n-1)
   such that exactly one of n and a(n) is prime and the other is composite. */

#include <stdbool.h>
#include <stdlib.h>
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
    /* cached values: cache[0] = a(1), cache[1] = a(2), ... */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n == 0) /* offset not used, but protect against 0 */
        return (seq_t)0;

    if (n > cache_len) {
        /* extend cache size to n (1‑based) */
        seq_t *newcache = mem_resize(seq_t, cache, (size_t)n);
        if (!newcache) {
            /* allocation failed – abort, but sequence requested only up to 10000 */
            abort();
        }
        cache = newcache;

        /* compute missing terms */
        for (idx_t i = cache_len; i < n; ++i) {
            if (i == 0) {            /* a(1) */
                cache[0] = (seq_t)1;
                continue;
            }

            idx_t idx = i + 1;        /* current n */
            seq_t prev = cache[i - 1];
            seq_t cand = prev + 1;

            bool nPrime = is_prime_big((seq_t)idx);

            /* search for the smallest candidate > prev satisfying the prime/composite rule */
            while (1) {
                bool candPrime = is_prime_big(cand);
                if (nPrime ^ candPrime) {
                    cache[i] = cand;
                    break;
                }
                cand++;
            }
        }
        cache_len = n;
    }

    return cache[n - 1];
}
