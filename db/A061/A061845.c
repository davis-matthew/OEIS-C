/*
 * A061845  Numbers that have one of every digit in some base.
 * https://oeis.org/A061845
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, nice, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 252, u32 n <= 7953, u64 n <= 7953,
 *            u128 n <= 7953, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that have one of every digit in some base (pandigital numbers). */
/* This implementation generates all base‑b pandigital numbers
 * (with leading digit ≠ 0) for b ≥ 2, merges them in increasing
 * order and returns the n‑th term (a(2)=2).  The cache is grown
 * incrementally on the first call that requires more terms. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper to store the generated numbers in a dynamic array. */
static seq_t *cache     = NULL; /* sorted array of generated terms          */
static size_t cache_len = 0;    /* number of elements currently stored      */
static size_t cache_cap = 0;    /* current capacity of the array            */

/* Cumulative count of terms that have been generated so far. */
static idx_t cumulative = 0;
/* The largest base for which pandigital numbers have been generated. */
static int last_generated_base = 1;  /* nothing yet */

/* Precomputed factorials up to 10! (fits in 64 bits). */
static const idx_t fact[12] = {
    1,                              /* 0! */
    1,                              /* 1! */
    2,                              /* 2! */
    6,                              /* 3! */
    24,                             /* 4! */
    120,                            /* 5! */
    720,                            /* 6! */
    5040,                           /* 7! */
    40320,                          /* 8! */
    362880,                         /* 9! */
    3628800,                        /*10!*/
    39916800                        /*11!  (unused for n≤10000) */
};

/* Append a value to the cache, growing the array if necessary. */
static void append_value(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp) abort();
        cache = newp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Recursive generation of base‑b pandigital numbers. */
static void gen_rec(int base, int depth, seq_t current, uint16_t used_mask)
{
    if (depth == base) {
        append_value(current);
        return;
    }
    for (int d = 0; d < base; ++d) {
        if (used_mask & (1U << d)) continue;
        if (depth == 0 && d == 0) continue;   /* no leading zero */
        gen_rec(base, depth + 1,
                 current * (seq_t)base + (seq_t)d,
                 used_mask | (1U << d));
    }
}

/* Generate all pandigital numbers for a given base and append them to the cache. */
static void generate_base(int base)
{
    gen_rec(base, 0, (seq_t)0, 0U);
}

/* Comparator for qsort (ascending order). */
static int cmp_seq_t(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* behaviour below the offset is irrelevant   */

    /* Generate bases until we have at least n‑1 terms.  The terms are
     * indexed from a(2) (i.e., cache[0]). */
    while (cumulative < n - 1) {
        /* Move to next base */
        ++last_generated_base;
        /* Generate all pandigital numbers for this base. */
        generate_base(last_generated_base);
        /* Update cumulative term count. */
        cumulative += fact[last_generated_base - 1];
        /* Keep the cache sorted.  The size is small (< 50 000), so a single
         * qsort is inexpensive. */
        qsort(cache, cache_len, sizeof(seq_t), cmp_seq_t);
    }
    /* Return the desired element (offset 2 → cache index 0). */
    return cache[n - 2];
}
