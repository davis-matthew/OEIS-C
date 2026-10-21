/*
 * A090028  Number of distinct lines through the origin in 6-dimensional cube
 *          of side length n.
 * https://oeis.org/A090028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   // a(n) = (n+1)^6 - 1 - Sum_{j=2..n+1} a( floor(n/j) ) // //
 *            Uses the recurrence with memoisation. For n=0 the value is 0
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Forward declaration of the sequence function. */
seq_t a(idx_t n);

/* Cached values of a(n).  The cache grows on demand. */
static seq_t *a_cache = NULL;
static idx_t  cache_cap = 0;

/* ------------------------------------------------------------------ */
/* Utility: compute x^6 using only seq_t arithmetic. */
static seq_t pow6(seq_t x)
{
    seq_t y = x;
    for (int i = 1; i < 6; ++i) y *= x;
    return y;
}

/* ------------------------------------------------------------------ */
/* Grow the cache so that indices up to 'need' are valid.  New
   elements are initialised to zero.  Works for both C and C++. */
static void grow_cache(idx_t need)
{
    if (need < cache_cap) return;

    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap <= need) new_cap <<= 1;

    /* Reallocate the array.  C allows a cast, C++ requires one too. */
    seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cache = tmp;

    /* Initialise newly allocated slots to zero. */
    for (idx_t i = cache_cap; i < new_cap; ++i) a_cache[i] = 0;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.  Computes a(n) and memoises the result. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) = 0 by definition */

    grow_cache(n + 1);             /* ensure indices 0..n are available */

    /* Compute all values up to n that are still missing. */
    for (idx_t i = 1; i <= n; ++i)
    {
        if (a_cache[i] != 0) continue;  /* already computed */

        /* (i+1)^6 - 1 ------------------------------------------------ */
        seq_t total = pow6((seq_t)(i + 1));
        total -= 1;

        /* Sum_{j=2..i+1} a(floor(i/j)) -------------------------------- */
        seq_t sum = 0;
        idx_t l = 2;
        while (l <= i + 1)
        {
            idx_t v = i / l;           /* floor(i/l) */
            idx_t r;
            if (v == 0)
                r = i + 1;             /* all remaining j give v=0 */
            else
            {
                r = i / v;             /* last j with this v */
                if (r > i + 1) r = i + 1;
            }

            /* add (r-l+1) * a(v) */
            sum += (seq_t)(r - l + 1) * a_cache[v];

            l = r + 1;
        }

        a_cache[i] = total - sum;
    }

    return a_cache[n];
}
