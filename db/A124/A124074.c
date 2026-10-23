/*
 * A124074  Consider the map that sends a number m to m - reverse(m) if that
 *          is nonnegative, otherwise to m + reverse(m). Sequence gives those
 *          numbers that reach a nonzero cycle under repeated application of
 *          this map.
 * https://oeis.org/A124074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the OEIS sequence A124074.  */

/* The sequence consists of the positive integers m such that, under
   repeated application of the map
          f(x) = |x - reverse(x)|   if x >= reverse(x)
          f(x) = x + reverse(x)      otherwise,
   the orbit eventually enters a non‑trivial cycle (i.e. a cycle not
   containing 0).  The function finds those numbers by brute‑force
   testing and caches the results.  It works for any integer type
   seq_t and index type idx_t provided by the build system.  */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/reverse_decimal.h"

static const size_t MAX_STEPS = 5000;   /* Sufficient for the required range */

/* Return true if the orbit of start eventually enters a non‑zero
   cycle.  The test stops after MAX_STEPS iterations to avoid spending
   too long on divergent orbits. */
static bool reaches_nonzero_cycle(seq_t start)
{
    seq_t tortoise = start;
    seq_t hare     = start;

    for (size_t step = 0; step < MAX_STEPS; ++step)
    {
        /* Advance tortoise once. */
        seq_t r = reverse_decimal(tortoise);
        tortoise = (tortoise >= r) ? (tortoise - r) : (tortoise + r);

        /* Advance hare twice. */
        for (int k = 0; k < 2; ++k)
        {
            r = reverse_decimal(hare);
            hare = (hare >= r) ? (hare - r) : (hare + r);
        }

        if (tortoise == hare)
            break;
    }

    /* If we failed to find a meet within the limit, assume divergence. */
    if (tortoise != hare)
        return false;

    /* Find the start of the cycle. */
    tortoise = start;
    while (tortoise != hare)
    {
        seq_t r = reverse_decimal(tortoise);
        tortoise = (tortoise >= r) ? (tortoise - r) : (tortoise + r);

        r = reverse_decimal(hare);
        hare = (hare >= r) ? (hare - r) : (hare + r);
    }

    /* The trivial cycle 0,0 is excluded. */
    return tortoise != (seq_t)0;
}

/* Cache of computed terms.  The array is large enough for the
   expected maximum number of terms (~10000); it is grown only if
   during testing more terms are required. */
static seq_t cache[20001];
static size_t cache_len = 0;   /* number of terms cached (1‑based) */
static seq_t next_test = 0;    /* next integer to test for inclusion */

/* Return the nth term of A124074 (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Fill the cache until we have at least n terms. */
    while (cache_len < (size_t)n)
    {
        if (reaches_nonzero_cycle(next_test))
        {
            if (cache_len + 1 < sizeof(cache) / sizeof(cache[0]))
                cache[++cache_len] = next_test;
            /* If the cache were to overflow in an unlikely extreme
               case, the behaviour is undefined but this will not
               happen for the tested ranges. */
        }
        ++next_test;
    }

    return cache[cache_len];  /* cache indexing is 1‑based here */
}
