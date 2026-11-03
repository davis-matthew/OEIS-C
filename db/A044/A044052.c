/*
 * A044052  Numbers k such that string 0,0 occurs in the base 3 representation
 *          of k but not of k-1.
 * https://oeis.org/A044052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 9 * m, where m is the nth positive integer such that the
 *            base‑3 representation of m‑1 contains no consecutive zeros
 * Range:     u8 n <= 25, u16 n <= 3861, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9779 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: test whether an unsigned integer has no consecutive zeros in its base‑3 representation. */
static bool no_consecutive_zeros(idx_t x)
{
    if (x == 0)
        return true;
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 3);
        if (prev == 0 && d == 0)
            return false;
        prev = d;
        x /= 3;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /*  Cache of already computed terms (1‑based index). */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t terms_generated = 0;      /* number of terms stored in cache */
    static idx_t next_m = 1;               /* next m to test (k / 9) */

    if (cache_cap < (size_t)n) {   /* enlarge cache when needed */
        seq_t *tmp = mem_resize(seq_t, cache, (size_t)n);
        if (!tmp)
            abort(); /* memory error */
        cache = tmp;
        cache_cap = (size_t)n;
    }

    /* Generate terms until we have at least n terms. */
    while (terms_generated < n) {
        /* Find the next m such that (m‑1) has no consecutive zeros in base‑3. */
        while (!no_consecutive_zeros(next_m - 1))
            ++next_m;

        /* Compute the sequence value and store it. */
        cache[terms_generated] = (seq_t)9 * (seq_t)next_m;
        ++terms_generated;
        ++next_m; /* advance to the next candidate for future calls */
    }

    return cache[n - 1];
}
