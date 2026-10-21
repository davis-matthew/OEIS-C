/*
 * A224552  Number of (n+2) X 3 0..1 matrices with each 3 X 3 subblock
 *          idempotent.
 * https://oeis.org/A224552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - a(n-4) - a(n-5) + a(n-6)
 *            for n > 7; with initial values a(1)=50, a(2)=70, a(3)=113,
 *            a(4)=172, a(5)=263, a(6)=418, a(7)=669
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *seq_cache   = NULL;  /* dynamic array of computed terms */
static size_t cache_capacity = 0;   /* current allocated size (>= cache_computed+1) */
static idx_t cache_computed  = 0;   /* largest n for which seq_cache[n] is valid */

/* initial values (index 0 unused) */
static const seq_t init_vals[8] = {
    (seq_t)0,          /* 0 – unused */
    (seq_t)50,         /* a(1) */
    (seq_t)70,         /* a(2) */
    (seq_t)113,        /* a(3) */
    (seq_t)172,        /* a(4) */
    (seq_t)263,        /* a(5) */
    (seq_t)418,        /* a(6) */
    (seq_t)669         /* a(7) */
};

/* Ensure the cache has capacity for at least n, and all values up to n are computed. */
static void ensure_cache(idx_t n)
{
    /* Grow storage if necessary.  Use exponential growth to keep overall cost O(n). */
    if ((size_t)n + 1 > cache_capacity) {
        size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
        while (new_cap <= (size_t)n + 1) {
            new_cap <<= 1;
        }
        seq_t *new_ptr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_ptr) {            /* allocation failure – not expected in tests   */
            return;
        }
        seq_cache = new_ptr;
        cache_capacity = new_cap;
    }

    /* Place the constants for indices 1..7 (no harm if they are already set). */
    for (idx_t i = 1; i <= 7 && i <= n; ++i)
        seq_cache[i] = init_vals[i];

    /* Compute any missing values up to n. */
    idx_t start = cache_computed + 1;
    if (start < 1) start = 1;
    for (idx_t i = start; i <= n; ++i) {
        if (i <= 7) continue; /* already initialized */

        /* Compute using the recurrence with only non‑negative intermediate values. */
        seq_t pos = 3 * seq_cache[i - 1] + 2 * seq_cache[i - 3] + seq_cache[i - 6];
        seq_t neg = 3 * seq_cache[i - 2] + seq_cache[i - 4] + seq_cache[i - 5];
        seq_cache[i] = pos - neg;  /* guaranteed non‑negative for all n */
    }
    cache_computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, but never requested */
    ensure_cache(n);
    return seq_cache[n];
}
