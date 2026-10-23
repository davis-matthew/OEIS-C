/*
 * A158725  Non-repdigit composite numbers not divisible by 2, 3, 5 or 11.
 * https://oeis.org/A158725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 8538, u32 n <= 9999, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, ok_alt_sum_diff, ok_digit_root,
 *            ok_divisor_filters, ok_last_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th element of OEIS A158725.
 * The sequence is defined as:
 *   non‑repeated “composite” numbers that are not divisible by 2, 3, 5 or 11,
 *   end in 1, 3, 7 or 9, have a digital root not a multiple of 3,
 *   and whose alternate‑digit sums differ by a number not divisible by 11.
 * A value is produced by a linear search with memoisation, so that
 * calls in arbitrary order are O(1) after the first few terms have been
 * generated. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/ok_alt_sum_diff.h"
#include "../utils/ok_digit_root.h"
#include "../utils/ok_divisor_filters.h"
#include "../utils/ok_last_digit.h"

static bool is_repdigit(seq_t x)
{
    seq_t first = x % 10;
    seq_t t = x / 10;
    while (t > 0) {
        if (t % 10 != first)
            return false;
        t /= 10;
    }
    return true;
}

/* ---------- memoised search ------------------------------------- */
static size_t cache_len  = 0;     /* number of terms stored */
static size_t cache_cap  = 0;     /* current array capacity */
static seq_t *cache       = NULL;
static idx_t last_candidate = 0;  /* last integer processed (as idx_t) */

/* Append a value to the cache, growing the array if needed. */
static void cache_append(seq_t val)
{
    if (cache_len == 0) {
        /* first allocation */
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    } else if (cache_len == cache_cap) {
        /* grow */
        cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
    }
    cache[cache_len++] = val;
}

/* Core entry point.  `n` is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; callers will not use n==0 */

    /* Continue building the cache until it covers index n. */
    while (cache_len < n) {
        last_candidate++;                /* 1, 2, 3, …  */
        seq_t cand = (seq_t)last_candidate;

        if (!ok_last_digit(cand))           continue;
        if (!ok_divisor_filters(cand))      continue;
        if (is_repdigit(cand))              continue;
        if (!ok_digit_root(cand))           continue;
        if (!ok_alt_sum_diff(cand))         continue;
        if (is_prime_big(cand))             continue;   /* needs to be composite */

        /* Store the accepted composite. */
        cache_append(cand);
    }

    return cache[n - 1];
}
