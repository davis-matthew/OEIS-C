/*
 * A209937  E.g.f. A(x) satisfies: A( x - Sum_{n>=2} x^n/(n*(n-1)/2) ) = x.
 * https://oeis.org/A209937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-2)*a(n-1) + Σ_{j=1}^{n-1} binom(n,j)*a(j)*a(n-j) OEIS
 *            A209937 (offset 1). The function caches computed values for
 *            subsequent calls. It works for any seq_t and idx_t supplied by
 *            the build. No external declarations of seq_t or idx_t are made
 *            here
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cache for computed terms: index 0 is unused, a[1] is the first term. */
static seq_t *seq_cache = NULL;   /* array of a[1..] */
static size_t cache_cap = 0;      /* current capacity of seq_cache (>= 1) */
static idx_t  cache_max = 0;      /* highest n that has been computed */

/* Ensure the cache can hold values up to and including n.  This also
 * guarantees that seq_cache[0] stays zero for consistency with C/
 * C++ indices, even though a(0) is undefined for this sequence. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 > cache_cap) {              /* need at least n+1 slots */
        size_t new_cap = (size_t)n + 1;
        if (cache_cap == 0) {                     /* first allocation */
            seq_cache = (seq_t*)calloc(new_cap, sizeof(seq_t));
        } else {
            seq_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        }
        cache_cap = new_cap;
    }

    if (cache_max == 0) {                         /* initialize a(1) */
        seq_cache[1] = (seq_t)1;                 /* a(1) = 1 */
        cache_max = 1;
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)              /* offset is 1 – behaviour below is irrelevant */
        return (seq_t)0;

    ensure_cache(n);

    if (n <= cache_max)            /* already computed */
        return seq_cache[n];

    /* Compute terms from cache_max+1 up to n.  All calculations use only
     * integer arithmetic on seq_t; no floating point or libraries are
     * required. */
    for (idx_t m = cache_max + 1; m <= n; ++m) {
        /* (m-2)*a(m-1) part */
        seq_t term = (seq_t)(m - 2) * seq_cache[m - 1];

        /* Sum over j, exploiting symmetry: Σ binom(m,j)*a(j)*a(m-j). */
        seq_t sum  = 0;
        seq_t bin  = 1;                  /* C(m,0) = 1 */
        idx_t half = m >> 1;             /* floor(m/2) */

        for (idx_t j = 1; j <= half; ++j) {
            /* Update binomial coefficient C(m,j) = C(m,j-1)*(m-j+1)/j */
            bin = bin * (seq_t)(m - j + 1);
            bin = bin / (seq_t)j;

            /* Product = C(m,j) * a(j) * a(m-j) */
            seq_t prod = bin * seq_cache[j] * seq_cache[m - j];

            /* Add both (j, m-j) and (m-j, j) unless they are the same */
            if (j != m - j)
                sum += prod + prod;
            else
                sum += prod;            /* middle term for even m */
        }

        seq_cache[m] = term + sum;
    }

    cache_max = n;
    return seq_cache[n];
}
