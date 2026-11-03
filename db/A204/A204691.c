/*
 * A204691  Numbers n such that n contains exactly 5 digits, all distinct, and
 *          n^2 contains exactly 9 distinct digits.
 * https://oeis.org/A204691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   a(n) is the nth 5‑digit integer with all distinct digits for
 *            which its square has exactly 9 distinct decimal digits. Computed
 *            by a brute‑force scan of the interval [10000,99999]. The list is
 *            built once on first call and reused afterwards
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: full, pass, 30 terms through n = 30 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Check that a 5‑digit number has all distinct digits */
static bool distinct_five_digits(idx_t n)
{
    unsigned mask = 0;
    for (int i = 0; i < 5; ++i) {
        unsigned d = n % 10;
        unsigned bit = 1u << d;
        if (mask & bit)
            return false;
        mask |= bit;
        n /= 10;
    }
    return true;
}

/* Check that a 64‑bit integer has exactly nine distinct decimal digits */
static bool distinct_nine_digits_sq(uint64_t sq)
{
    unsigned mask = 0;
    int distinct = 0;
    while (sq > 0) {
        unsigned d = sq % 10;
        unsigned bit = 1u << d;
        if (!(mask & bit)) {
            mask |= bit;
            ++distinct;
            if (distinct > 9)
                return false;
        }
        sq /= 10;
    }
    return distinct == 9;
}

static seq_t *cache = NULL;      /* array of found numbers                 */
static size_t cache_size = 0;    /* how many numbers are stored            */
static size_t cache_cap  = 0;    /* current capacity                       */

/* Build the cache on first use */
static void build_cache(void)
{
    if (cache != NULL)
        return;   /* already built */

    /* Initial capacity: 64, grow by factor 2 if necessary */
    size_t cap = 64;
    cache = mem_new(seq_t, cap);
    if (!cache) abort();   /* out of memory */

    for (idx_t num = 10000; num <= 99999; ++num) {
        if (!distinct_five_digits(num))
            continue;

        uint64_t sq = (uint64_t)num * (uint64_t)num;
        if (!distinct_nine_digits_sq(sq))
            continue;

        /* Append to cache */
        if (cache_size == cap) {
            cap *= 2;
            seq_t *tmp = mem_resize(seq_t, cache, cap);
            if (!tmp) abort();
            cache = tmp;
        }
        cache[cache_size++] = (seq_t)num;
    }
}

seq_t a(idx_t n)
{
    /* Ensure the cache is populated */
    build_cache();

    /* Sequence is 1‑based; return 0 for out‑of‑range indices */
    if ((size_t)n == 0 || (size_t)(n - 1) >= cache_size)
        return (seq_t)0;

    return cache[n - 1];
}
