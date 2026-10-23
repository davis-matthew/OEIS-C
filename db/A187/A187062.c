/*
 * A187062  Expansion of 2*x^2 *(4 +7*x +5*x^2 -x^3 -4*x^4 +6*x^6 +4*x^7 -x^8
 *          -2*x^9) / ((1+x)^2 *(1+x+x^2)^2 *(1-x)^4).
 * https://oeis.org/A187062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + 2*a(n-3) - a(n-4) - 4*a(n-5) - a(n-6) +
 *            2*a(n-7) + 2*a(n-8) - a(n-10) (n ≥ 13); For n = 11,12 a constant
 *            RHS from the numerator is needed: a(11) =
 *            2*a(9)+2*a(8)-a(7)-4*a(6)-a(5)+2*a(4)+2*a(3)-a(1) - 2; a(12) =
 *            2*a(10)+2*a(9)-a(8)-4*a(7)-a(6)+2*a(5)+2*a(4)-a(2) - 4; /* The
 *            first...
 * Range:     u8 n <= 11, u16 n <= 73, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[0] unused, cache[i] = a(i) */
static idx_t cache_cap = 0;      /* size allocated (including cache[0]) */
static idx_t cache_max = 0;      /* highest index computed so far */

/* Ensure that cache has room for indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    cache = tmp;
    cache_cap = n + 1;
}

seq_t a(idx_t n)
{
    /* First‑time initialization: allocate and set the first ten terms. */
    if (!cache)
    {
        /* indices 0..10 (cache[0] unused) */
        cache = (seq_t *)calloc(11, sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure */
        cache_cap = 11;
        cache[1] = 0;   /* a(1) */
        cache[2] = 0;   /* a(2) */
        cache[3] = 8;   /* a(3) */
        cache[4] = 14;  /* a(4) */
        cache[5] = 26;  /* a(5) */
        cache[6] = 42;  /* a(6) */
        cache[7] = 64;  /* a(7) */
        cache[8] = 90;  /* a(8) */
        cache[9] = 134; /* a(9) */
        cache[10] = 172;/* a(10) */
        cache_max = 10;
    }

    if (n <= cache_max)
        return cache[n];

    ensure_cache(n);

    /* Compute missing terms in order up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
        if (i <= 10)   /* already initialized */
            continue;

        /* Common part of the recurrence. */
        seq_t val = cache[i-2] + cache[i-2]
                  + cache[i-3] + cache[i-3]
                  - cache[i-4];
        val -= cache[i-5] + cache[i-5] + cache[i-5] + cache[i-5];
        val -= cache[i-6];
        val += cache[i-7] + cache[i-7];
        val += cache[i-8] + cache[i-8];
        val -= cache[i-10];

        /* Adjust for the two exceptional indices. */
        if (i == 11) val -= (seq_t)2;   /* RHS = -2 */
        else if (i == 12) val -= (seq_t)4;   /* RHS = -4 */

        cache[i] = val;
    }

    cache_max = n;
    return cache[n];
}
