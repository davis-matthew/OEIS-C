/*
 * A069105  1/n has period 3 in base 10.
 * https://oeis.org/A069105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 123, u32 n <= 348, u64 n <= 348,
 *            u128 n <= 348, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A069105: Numbers of the form a*2^i*5^j with a ∈ {27,37,111,333,999}. */

/* Compute the n‑th term (a(1) is the first term).  The function
   memoises previously computed terms so that calls in any order are
   handled efficiently. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----- global memoisation state ----- */
static seq_t *cache = NULL;          /* cache[n-1] holds a(n) */
static size_t cache_cap = 0;
static size_t cache_size = 0;

static seq_t *ugly = NULL;           /* ugly[k] = 2^i * 5^j  */
static size_t ugly_cap = 0;
static size_t ugly_size = 0;
static size_t p2 = 0, p5 = 0;        /* indices used while generating ugly numbers */

/* indices into each of the 5 a*ugly sequences */
static size_t cur_idx[5] = {0,0,0,0,0};
static const seq_t a_vals[5] = {27, 37, 111, 333, 999};

/* ----- helper: grow dynamic array for any seq_t ----- */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

static void grow_ugly(size_t needed)
{
    if (ugly_cap >= needed) return;
    size_t new_cap = (ugly_cap > 0) ? ugly_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(ugly, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    ugly = tmp;
    ugly_cap = new_cap;
}

/* ----- generate the next ugly number (2^i * 5^j) ----- */
static void next_ugly(void)
{
    seq_t v2 = ugly[p2] * (seq_t)2;
    seq_t v5 = ugly[p5] * (seq_t)5;
    seq_t nxt = (v2 < v5) ? v2 : v5;
    if (v2 == v5) {
        ++p2; ++p5;
    } else if (v2 < v5) {
        ++p2;
    } else {
        ++p5;
    }
    grow_ugly(ugly_size + 1);
    ugly[ugly_size++] = nxt;
}

/* ----- initialise global arrays once ----- */
static void init_globals(void)
{
    if (cache) return;                       /* already initialised */
    cache = NULL; cache_cap = 0; cache_size = 0;
    ugly = NULL; ugly_cap = 0; ugly_size = 0;
    p2 = p5 = 0;
    ugly = (seq_t *)malloc(sizeof(seq_t));
    if (!ugly) abort();
    ugly[0] = (seq_t)1;                      /* 2^0 * 5^0 */
    ugly_size = 1;
    for (size_t i = 0; i < 5; ++i) cur_idx[i] = 0;
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined, but harmless */
    init_globals();

    /* Generate up to the nth term if needed. */
    while (cache_size < (size_t)n) {
        /* Find the smallest candidate among the 5 sequences. */
        seq_t min_val = 0;
        size_t min_a  = 0;
        bool first = true;

        for (size_t k = 0; k < 5; ++k) {
            size_t idx = cur_idx[k];
            if (idx == ugly_size) next_ugly();          /* ensure ugly[idx] exists */
            seq_t val = a_vals[k] * ugly[idx];          /* a * 2^i * 5^j */

            if (first || val < min_val) {
                min_val = val;
                min_a  = k;
                first = false;
            }
        }

        /* Add to cache if it is not a duplicate. */
        if (cache_size == 0 || min_val != cache[cache_size - 1]) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = min_val;
        }

        /* Advance the chosen sequence. */
        ++cur_idx[min_a];
    }

    return cache[n - 1];
}
