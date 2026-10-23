/*
 * A118393  Eigenvector of triangle A059344. E.g.f.: exp( Sum_{n>=0} x^(2^n)
 *          ).
 * https://oeis.org/A118393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A118393 – Eigenvector of triangle A059344
 * Recurrence, OEIS notation:
 *     a(0) = 1
 *     a(n) = Σ_{k=0..⌊n/2⌋} (n! / (k! (n-2k)!)) * a(k)   for n ≥ 0
 */

#include <stdlib.h>   /* malloc, realloc, free, size_t */

/* seq_t and idx_t are defined by the build system */
static seq_t *cache = NULL;      /* array of computed a(i) */
static idx_t cache_cap = 0;      /* allocated capacity (elements) */
static idx_t computed_max = 0;   /* highest index whose value is known */

/*--------------------------------------------------------------------
 * Ensure that cache has space for indices 0..n.  Allocate or grow the
 * array as necessary and initialise any new elements to zero.  If the
 * array is created for the first time, initialise a(0) = 1.
 *--------------------------------------------------------------------*/
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;

    size_t old_cap = cache_cap ? cache_cap : 1;
    size_t new_cap = old_cap;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – abort the program */
        exit(1);
    }
    /* If we grew the array, zero the new elements */
    if (new_cap > old_cap) {
        size_t i;
        for (i = old_cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
    }
    cache = tmp;
    cache_cap = new_cap;

    /* The first element is always a(0) = 1.  Set it here on first
       allocation or if the array was previously all zeros. */
    if (computed_max == 0 && cache[0] == (seq_t)0)
        cache[0] = (seq_t)1;
}

/*--------------------------------------------------------------------
 * Compute a(n) for arbitrary n ≥ 0.  The function is memoised and
 * works correctly for calls in any order.
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Make sure the cache can hold the desired index */
    ensure_cache(n);

    /* Compute all values up to n that are still missing */
    idx_t start = computed_max + 1;
    if (start == 0)         /* ensure start is at least 1, a(0) already set */
        start = 1;
    if (n < start)          /* the requested value is already known */
        return cache[n];

    for (idx_t i = start; i <= n; ++i) {
        idx_t maxk = i >> 1;          /* floor(i/2) */
        seq_t sum   = (seq_t)0;
        seq_t coeff = (seq_t)1;       /* coefficient for k=0 : n!/(0!(n)!) = 1 */

        for (idx_t k = 0; k <= maxk; ++k) {
            /* Add current term: coeff * a(k) */
            sum = sum + coeff * cache[k];

            if (k == maxk)
                break;

            /* Prepare coefficient for next k+1 */
            /* coeff_{k+1} = coeff_k * (i - 2k) * (i - 2k - 1) / (k + 1) */
            seq_t m1 = (seq_t)(i - 2*k);
            seq_t m2 = m1 - 1;            /* i - 2k - 1 */
            coeff = coeff * m1;
            coeff = coeff * m2;
            coeff = coeff / (seq_t)(k + 1);   /* exact division */
        }

        cache[i] = sum;
    }

    computed_max = n;
    return cache[n];
}
