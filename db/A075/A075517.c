/*
 * A075517  Created by removing all integers which take an odd number of
 *          nested digit sums to reach <10.
 * https://oeis.org/A075517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A075517 – numbers that require an even number of
   nested digit‑sum reductions to become a single digit.
   a(0) = 0, a(1) = 1, … */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;   /* stored terms */
static idx_t  cap   = 0;      /* allocated capacity */
static idx_t  sz    = 0;      /* count of stored terms */
static idx_t  next_candidate = 0;

/* Ensure cache can hold at least 'needed' elements. */
static void ensure_cap(idx_t needed)
{
    if (needed <= cap) return;
    idx_t new_cap = (cap == 0) ? 8 : cap * 2;
    if (new_cap < needed) new_cap = needed;
    cache = seq_realloc(cache, (size_t)cap, (size_t)new_cap);
    cap = new_cap;
}

/* Sum of decimal digits of a non‑negative integer (idx_t). */
static idx_t digit_sum_idx(idx_t v)
{
    idx_t s = 0;
    while (v > 0) {
        s += v % 10;
        v /= 10;
    }
    return s;
}

/* Generate terms up to index 'n' (inclusive). */
static void generate_upto(idx_t n)
{
    while (sz <= n) {
        idx_t x = next_candidate;
        unsigned int steps = 0;
        while (x >= 10) {
            x = digit_sum_idx(x);
            ++steps;
        }
        if ((steps & 1u) == 0) {          /* even number of steps → keep */
            ensure_cap(sz + 1);
            cache[sz++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* Sequence entry a(n). */
seq_t a(idx_t n)
{
    generate_upto(n);
    return cache[n];
}
