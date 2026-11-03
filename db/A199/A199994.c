/*
 * A199994  Composite numbers whose multiplicative persistence is 4.
 * https://oeis.org/A199994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth composite number whose multiplicative persistence
 *            equals 4
 * Range:     u8 n <= 2, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   digit_product, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches numbers sequentially, testing each for
 * a multiplicative persistence of 4 and compositeness. Matching numbers
 * are stored in a cache so that multiple unordered calls are fast. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/digit_product.h"
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t *cache = NULL;      /* cached sequence terms */
static idx_t  cache_len = 0;     /* number of cached terms           */
static size_t cache_cap = 0;     /* allocated capacity of cache       */
static idx_t  last_checked = 0;  /* last integer examined            */

/* Return the nth term (1‑based) of sequence A199994. */
seq_t a(idx_t n)
{
    /* N is guaranteed ≥ 1 by caller; guard against accidental 0 */
    if (n == 0) return 0;

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        last_checked++;                                    /* next candidate */

        /* Skip numbers < 2 – they cannot be composite. */
        if (last_checked < 2) continue;

        /* Compute multiplicative persistence. */
        seq_t x = last_checked;
        int steps = 0;
        while (x >= 10) {                                   /* more than one digit */
            if (++steps > 4) break;                         /* too many steps */
            x = digit_product(x);
        }
        if (steps != 4) continue;                          /* persistence ≠ 4 */

        /* Composite? (i.e., not prime). */
        if (is_prime_big(last_checked)) continue;

        /* Store the found term in the cache. */
        if (cache_len == cache_cap)
            grow_seq(&cache, &cache_cap, cache_len + 1);
        cache[cache_len++] = last_checked;
    }

    /* Return the requested term (0‑based indexing). */
    return cache[n - 1];
}
