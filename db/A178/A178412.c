/*
 * A178412  a(n) is defined recursively as the Sum{d|n} ((-1)^(n/d))*a(d) = 1,
 *          with a(1) = a(2) = 1.
 * https://oeis.org/A178412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   For n ≥ 3, a(n) = Σ_{d|n, d<n} (-1)^{n/d} · a(d) – 1 Initial
 *            values: a(1) = 1, a(2) = 1 The function memoises all computed
 *            terms, so calls can be in any order. This implementation works
 *            for both fixed‑size signed integers and arbitrary‑precision
 *            types
 * Range:     i8 n <= 383, i16 n <= 9671, i32 n <= 9671, i64 n <= 9671,
 *            i128 n <= 9671, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Memoisation storage.  Index 0 is unused. */
static seq_t *cache  = NULL;     /* computed a(i) values */
static uint8_t *seen = NULL;     /* 1 = value known, 0 = unknown */
static size_t cap = 0;           /* current array capacity (max index + 1) */

/* Ensure the arrays hold at least indices 0 … n.  Index 0 is unused. */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n <= cap) return;          /* already enough space */

    size_t new_cap = cap ? cap : 16;
    while (new_cap < (size_t)n) new_cap <<= 1;

    /* realloc works for both C and C++; cast the void* result. */
    seq_t *new_cache = (seq_t *)realloc((void *)cache,
                                        sizeof(seq_t) * (new_cap + 1));
    if (!new_cache) abort();              /* memory allocation failure */

    uint8_t *new_seen = (uint8_t *)realloc((void *)seen,
                                           sizeof(uint8_t) * (new_cap + 1));
    if (!new_seen) abort();               /* memory allocation failure */

    /* Initialise newly added entries as "unknown". */
    for (size_t i = cap + 1; i <= new_cap; ++i) new_seen[i] = 0;

    cache = new_cache;
    seen  = new_seen;
    cap   = new_cap;
}

/* Compute all a(i) for i = 1 … n, if not already known. */
static void compute_up_to(idx_t n)
{
    if (n == 0) return;                     /* sequence undefined for 0 */

    ensure_capacity(n);

    /* Initialise the fixed initial values. */
    if (!seen[1]) { cache[1] = (seq_t)1; seen[1] = 1; }
    if (!seen[2]) { cache[2] = (seq_t)1; seen[2] = 1; }

    for (idx_t i = 3; i <= n; ++i)
    {
        if (seen[i]) continue;              /* already computed */

        seq_t sum = 0;
        for (idx_t d = 1; d * d <= i; ++d)
        {
            if (i % d == 0)
            {
                /* divisor d */
                if (d < i)
                {
                    idx_t q = i / d;
                    if (q & 1) sum -= cache[d];
                    else      sum += cache[d];
                }

                /* complementary divisor e = i / d */
                idx_t e = i / d;
                if (e != d && e < i)
                {
                    idx_t q = i / e;         /* which equals d */
                    if (q & 1) sum -= cache[e];
                    else      sum += cache[e];
                }
            }
        }
        cache[i] = sum - (seq_t)1;          /* a(i) = S - 1 */
        seen[i]  = 1;
    }
}

/* External function required by the interface */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* undefined for n < 1, safe */
    compute_up_to(n);
    return cache[n];
}
