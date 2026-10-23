/*
 * A161958  a(n) = smallest composite number which is the sum of n consecutive
 *          composite numbers.
 * https://oeis.org/A161958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest composite number which is the sum of n *
 *            consecutive composite numbers. This implementation keeps a list
 *            * of composite numbers and their prefix sums, then scans for the
 *            * first composite sum of a given length. Results are cached * so
 *            that arbitrary calls to a(n) are efficient
 * Range:     u8 n <= 16, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Static storage for composite numbers and their prefix sums. */
static seq_t *comps = NULL;          /* array of composite numbers */
static seq_t *pref = NULL;           /* prefix sums: pref[0] = 0,
                                       * pref[i+1] = sum(comps[0..i]) */
static size_t comp_cap = 0;          /* current capacity of comps/pref pair */
static size_t comp_cnt = 0;          /* number of composites stored */
static seq_t next_candidate = 4;     /* next integer to test for compositeness */

/* Cache of computed a(n) values. */
static seq_t *a_cache = NULL;
static size_t cache_cap = 0;

/* Ensure the composite arrays have at least `min_cap` capacity. */
static void ensure_composite_capacity(size_t min_cap)
{
    if (comp_cap >= min_cap)
        return;
    size_t new_cap = comp_cap ? comp_cap * 2 : 64;
    if (new_cap < min_cap)
        new_cap = min_cap;
    comps = (seq_t *)realloc(comps, new_cap * sizeof(seq_t));
    pref  = (seq_t *)realloc(pref,  (new_cap + 1) * sizeof(seq_t));
    comp_cap = new_cap;
    /* Pref[0] must be 0. */
    if (pref != NULL)
        pref[0] = 0;
}

/* Generate composite numbers until we have at least `needed` composites. */
static void generate_composites(size_t needed)
{
    while (comp_cnt < needed) {
        /* Skip primes to find the next composite. */
        while (is_prime_big(next_candidate))
            next_candidate++;
        /* Store the composite. */
        ensure_composite_capacity(comp_cnt + 1);
        comps[comp_cnt] = next_candidate;
        pref[comp_cnt + 1] = pref[comp_cnt] + comps[comp_cnt];
        comp_cnt++;
        next_candidate++;
    }
}

/* Main function required by the build system. */
seq_t a(idx_t n)
{
    /* Offset: a(1) is the first term.  We ignore a(0). */
    if (n == 0)
        return 0;

    size_t idx = (size_t)n;  /* 1‑based index for the cache */

    /* Resize the cache if necessary. */
    if (idx >= cache_cap) {
        size_t new_cap = idx + 1;
        if (a_cache == NULL)
            a_cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
        else
            a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        /* Initialise new entries to 0 (sentinel). */
        if (cache_cap < new_cap)
            for (size_t i = cache_cap; i < new_cap; ++i)
                a_cache[i] = 0;
        cache_cap = new_cap;
    }

    /* Return cached value if it exists. */
    if (a_cache[idx] != 0)
        return a_cache[idx];

    /* Find the smallest composite sum of `idx` consecutive composites. */
    size_t start = 0;
    while (true) {
        size_t need = start + idx;  /* number of composites required */
        generate_composites(need);
        seq_t sum = pref[need] - pref[start];
        if (!is_prime_big(sum)) {   /* composite sum found */
            a_cache[idx] = sum;
            return sum;
        }
        ++start;
    }
}
