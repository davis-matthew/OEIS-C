/*
 * A023754  Plaindromes: numbers whose digits in base 13 are in nondecreasing
 *          order.
 * https://oeis.org/A023754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 144, u16 n <= 3681, u32 n <= 9000, u64 n <= 9000,
 *            u128 n <= 9000, big any n
 * Validated: standard, pass, 86 terms through n = 8758 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A023754 – Plaindromes.
   a(n) is the nth nonnegative integer whose base‑13 digits are
   in nondecreasing order (a(1)=0). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check whether the digits of x in base 13 are nondecreasing
   from most significant to least (i.e. nonincreasing when read
   from least significant to most).                 */
static bool is_nondecreasing_base13(seq_t x)
{
    if (x == (seq_t)0) return true;
    int prev = 13;                    /* greater than any digit */
    while (x > (seq_t)0) {
        int d = (int)(x % (seq_t)13); /* current least sign. digit */
        if (d > prev) return false;  /* would decrease in MSB→LSB order */
        prev = d;
        x /= (seq_t)13;
    }
    return true;
}

/* ---------- internal state ---------- */
static seq_t *cache          = NULL;  /* array of computed terms   */
static size_t cache_cap      = 0;     /* allocated size of cache   */
static idx_t cached_n        = 0;     /* number of entries stored  */
static seq_t next_candidate  = (seq_t)1; /* next value to test */

/* ------------------------------------------------------------------ */
/* Public function:  a(n)  (1‑based index)                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (cache == NULL) {                 /* first call: allocate cache */
        cache_cap = 32;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return (seq_t)0;     /* allocation failure */
        cache[0] = (seq_t)0;             /* a(1) = 0 */
        cached_n = 1;
        next_candidate = (seq_t)1;
    }

    /* Return cached value if already known. */
    if (n <= cached_n) return cache[n - 1];

    /* Generate terms until the requested index is reached. */
    while (cached_n < n) {
        /* Grow cache when full. */
        if (cached_n >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) return (seq_t)0;   /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        if (is_nondecreasing_base13(next_candidate)) {
            cache[cached_n] = next_candidate;
            ++cached_n;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
