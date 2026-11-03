/*
 * A175104  a(1)=1. For n >= 2, a(n) = 2*a(n-1) + (number of composites among
 *          first n-1 terms of the sequence).
 * https://oeis.org/A175104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = 2*a(n-1) + (number of composites among the
 *            first n-1 terms of the sequence)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 991 (2026-11-03)
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

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t n_max = 0;
static idx_t composites = 0;

seq_t a(idx_t n) {
    if (n < 1)
        return 0; /* undefined for indices below the offset */

    if (n > n_max) {
        /* Allocate or grow the cache to accommodate the new terms. */
        if (cache == NULL) {
            cache = mem_new(seq_t, n);
            cache_cap = n;
        } else if (n > cache_cap) {
            seq_t *tmp = mem_resize(seq_t, cache, n);
            if (!tmp) return 0; /* allocation failed */
            cache = tmp;
            cache_cap = n;
        }

        /* Generate terms from (n_max + 1) up to n. */
        for (idx_t i = n_max + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                val = (seq_t)1;
            } else {
                seq_t prev = cache[i - 2];
                val = 2 * prev + composites;
            }
            cache[i - 1] = val;

            /* Update composite count after adding the new term. */
            if (val != (seq_t)1 && !is_prime_big(val))
                composites++;
        }

        n_max = n;
    }

    return cache[n - 1];
}
