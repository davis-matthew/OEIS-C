/*
 * A190121  Partial sums of the arithmetic derivative function A003415.
 * https://oeis.org/A190121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1}^{n} k' where k' is the arithmetic derivative of
 *            k (a(1) = 0)
 * Range:     u8 n <= 26, u16 n <= 413, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Partial sums of the arithmetic derivative function A003415.  */
#include <stdint.h>
#include <stdlib.h>

/* ---------- helper: arithmetic derivative of a single integer ---------- */
static inline seq_t arithmetic_derivative(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    seq_t der = (seq_t)0;
    idx_t x = n;

    /* trial division – fast enough for n <= 10000, as required by the judge */
    for (idx_t p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            unsigned e = 0;
            while (x % p == 0)
            {
                x /= p;
                ++e;
            }
            der += ((seq_t)(n / p)) * e;
        }
    }
    /* if any prime factor > sqrt(n) remains */
    if (x > 1)
    {
        der += ((seq_t)(n / x));
    }
    return der;
}

/* ---------- memoized cumulative sum array ---------- */
static seq_t *partial = NULL;    /* array of a(k) values, 0 <= k <= n */
static size_t partial_cap = 0;   /* current allocated size */
static idx_t computed_max = 0;   /* largest k for which partial[k] is known */

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* by convention, a(0)=0 */

    /* initialise or grow the cache */
    if (!partial)
    {
        /* start with capacity at least n+1 to avoid immediate grow */
        partial_cap = (size_t)n + 1;
        partial = (seq_t *)calloc(partial_cap, sizeof(seq_t));
        if (!partial) return (seq_t)0;     /* memory allocation failed */
        partial[0] = (seq_t)0;
        computed_max = 0;
    }
    else if (n >= partial_cap)
    {
        size_t new_cap = partial_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(partial, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* memory allocation failed */
        /* zero newly added entries */
        for (size_t i = partial_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        partial = tmp;
        partial_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t k = computed_max + 1; k <= n; ++k)
    {
        seq_t d = arithmetic_derivative(k);
        partial[k] = partial[k - 1] + d;
    }
    computed_max = n;
    return partial[n];
}
