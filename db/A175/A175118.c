/*
 * A175118  a(1)=2. a(n) = the smallest prime p > a(n-1) such that p-a(n-1)+1
 *          is composite.
 * https://oeis.org/A175118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2; for n>1, a(n)=min{p prime > a(n‑1) : p-a(n‑1)+1 is
 *            composite}
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 985 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_sz = 0;

static void ensure_cache(idx_t need)
{
    if (need == 0) return;
    if (cache == NULL) {
        cache_cap = need;
        cache = mem_new(seq_t, cache_cap);
    } else if (need > cache_cap) {
        cache_cap = need;
        cache = mem_resize(seq_t, cache, cache_cap);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined below offset */

    ensure_cache(n);

    if (cache_sz == 0) {                    /* initialise first term */
        cache[0] = (seq_t)2;
        cache_sz = 1;
    }

    while (cache_sz < n) {                   /* generate terms up to n */
        seq_t prev = cache[cache_sz - 1];
        seq_t candidate = prev + (seq_t)1;

        while (1) {
            if (!is_prime_big(candidate)) {  /* not a prime */
                candidate++;
                continue;
            }
            /* candidate is prime – test the composite condition */
            seq_t diff = candidate - prev + (seq_t)1;
            if (!is_prime_big(diff))         /* composite diff -> accept */
                break;
            candidate++;                     /* look for next prime */
        }

        cache[cache_sz++] = candidate;        /* store the next term */
    }

    return cache[n - 1];
}
