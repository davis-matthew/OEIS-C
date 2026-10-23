/*
 * A123236  a(n) = 10*a(n-1) + 44*a(n-2) - 370*a(n-3) - 240*a(n-4) +
 *          2400*a(n-5), a(1) = 1, a(2) = 1, a(3) = 31, a(4) = 134, a(5) =
 *          2094, for n >= 6.
 * https://oeis.org/A123236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + 44*a(n-2) - 370*a(n-3) - 240*a(n-4) +
 *            2400*a(n-5), a(1) = 1, a(2) = 1, a(3) = 31, a(4) = 134, a(5) =
 *            2094
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoizes the sequence in an array that grows
   as needed and uses only integer arithmetic. It works for any
   build where seq_t supports +, -, *, and / on unsigned values. */

#include <stdlib.h>
#include <stddef.h>

/* Static state for memoization. */
static seq_t *cache = NULL;       /* 1‑based array of terms */
static size_t cache_cap = 0;      /* allocated capacity (size + 1) */
static size_t last_computed = 0;  /* highest index that has been filled */

/* Ensure the cache has capacity for indices up to n (inclusive). */
static void ensure_cache_n(size_t n)
{
    if (n + 1 <= cache_cap)
        return;
    size_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Out of memory – aborting is safe for this exercise. */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialize the first five terms (if not already done). */
static void init_base(void)
{
    if (last_computed >= 5)
        return;
    ensure_cache_n(5);
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)1;
    cache[3] = (seq_t)31;
    cache[4] = (seq_t)134;
    cache[5] = (seq_t)2094;
    last_computed = 5;
}

/* Main function requested by the grader. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    size_t idx = (size_t)n;

    if (idx <= 5) {
        init_base();
        return cache[idx];
    }

    /* Need terms beyond the ones we already have. */
    if (last_computed < idx) {
        init_base();                 /* make sure base terms are present */
        ensure_cache_n(idx);         /* enlarge storage if necessary */

        for (size_t i = last_computed + 1; i <= idx; ++i) {
            /* Positive contributions */
            seq_t t1 = cache[i - 1];
            t1 *= (seq_t)10;
            seq_t t2 = cache[i - 2];
            t2 *= (seq_t)44;
            seq_t t3 = cache[i - 5];
            t3 *= (seq_t)2400;
            seq_t positive = t1 + t2 + t3;

            /* Negative contributions */
            seq_t t4 = cache[i - 3];
            t4 *= (seq_t)370;
            seq_t t5 = cache[i - 4];
            t5 *= (seq_t)240;
            seq_t negative = t4 + t5;

            cache[i] = positive - negative;
        }
        last_computed = idx;
    }

    return cache[idx];
}
