/*
 * A207173  Number of 5 X n 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 38, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Empirical recurrence (valid for n > 11):
 *   a(n) = a(n-1) + 17*a(n-3) + 4*a(n-4) + 4*a(n-5)
 *           - 48*a(n-6) - 32*a(n-7) + 64*a(n-9)
 * Initial values:
 *   a(1)  = 10
 *   a(2)  = 100
 *   a(3)  = 270
 *   a(4)  = 1161
 *   a(5)  = 3445
 *   a(6)  = 8455
 *   a(7)  = 26908
 *   a(8)  = 82861
 *   a(9)  = 228060
 *   a(10) = 672760
 *   a(11) = 2029041
 *
 * This implementation memoises the sequence in a static array.
 * It handles calls in arbitrary order and works for arbitrary‑precision
 * seq_t types as long as the usual arithmetic operators are defined.
 */

#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* seq_t and idx_t are supplied by the build environment.          */
/* ------------------------------------------------------------------ */

/* Function to compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    /* Return 0 for n == 0 (outside the defined offset). */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Static cache of computed values.  cache[0] is unused. */
    static seq_t *cache = NULL;   /* pointer to sequence values   */
    static size_t cap   = 0;      /* allocated capacity (elements)*/
    static size_t len   = 0;      /* next free index (cache holds 0..len-1) */

    /* Initial allocation and seeding of known values. */
    if (len == 0) {
        /* Reserve space for indices 0..11 (12 elements). */
        cap = 12;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)
            abort(); /* out of memory */
        for (size_t i = 0; i < cap; ++i)
            cache[i] = (seq_t)0;

        cache[1]  = (seq_t)10;
        cache[2]  = (seq_t)100;
        cache[3]  = (seq_t)270;
        cache[4]  = (seq_t)1161;
        cache[5]  = (seq_t)3445;
        cache[6]  = (seq_t)8455;
        cache[7]  = (seq_t)26908;
        cache[8]  = (seq_t)82861;
        cache[9]  = (seq_t)228060;
        cache[10] = (seq_t)672760;
        cache[11] = (seq_t)2029041;
        len = 12; /* next index to compute */
    }

    /* Grow cache if necessary. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort(); /* out of memory */
        for (size_t i = cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing values up to n. */
    while (len <= (size_t)n) {
        idx_t i = (idx_t)len;          /* current index */
        seq_t val;
        if (i <= (idx_t)11) {
            val = cache[i];            /* already known */
        } else {
            /* Apply recurrence. */
            val = cache[i-1]
                + cache[i-3] * (seq_t)17
                + cache[i-4] * (seq_t)4
                + cache[i-5] * (seq_t)4
                - cache[i-6] * (seq_t)48
                - cache[i-7] * (seq_t)32
                + cache[i-9] * (seq_t)64;
        }
        cache[i] = val;
        ++len;
    }
    return cache[(size_t)n];
}
