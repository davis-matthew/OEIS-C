/*
 * A046459  Dudeney numbers: integers equal to the sum of the digits of their
 *          cubes.
 * https://oeis.org/A046459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: integers equal to the sum of the digits of their cubes (Dudeney numbers). */

#include <stdlib.h>

/* Static cache of found Dudeney numbers */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_test = 0;

/* Ensure the cache has at least <needed> capacity. */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t newcap = cache_cap ? cache_cap * 2 : 8;
    while (newcap < needed)
        newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) {
        /* If reallocation fails we simply abort; this is extremely unlikely for
         * the small numbers involved in this sequence. */
        return;
    }
    cache = tmp;
    cache_cap = newcap;
}

/* Compute the sum of decimal digits of a seq_t number. */
static uint64_t digit_sum_seq(seq_t v)
{
    uint64_t sum = 0;
    while (v) {
        sum += (uint64_t)(v % 10);
        v /= 10;
    }
    return sum;
}

/* Public function as required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0)        /* The OEIS offset is 1; if called with 0 we return 0. */
        return 0;

    while (cache_len < (size_t)n) {
        seq_t val   = (seq_t)next_test;
        seq_t cube  = val * val * val;
        uint64_t sum = digit_sum_seq(cube);
        if ((seq_t)sum == next_test) {
            ensure_cache_capacity(cache_len + 1);
            if (cache_cap > 0) {
                cache[cache_len++] = val;
            }
        }
        ++next_test;
    }
    return cache[(size_t)n - 1];
}
