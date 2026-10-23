/*
 * A126187  Sum of the levels of the first leaf (in the preorder traversal)
 *          over all hex trees with n edges.
 * https://oeis.org/A126187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A126187: Sum of the levels of the first leaf (preorder)
   over all hex trees with n edges.
   Recurrence: (n-1)*(3*n-1)*(n+4)*a(n) - n*(18*n^2+21*n-19)*a(n-1)
                +5*n*(3*n+2)*(n-1)*a(n-2)=0  for n≥3,
   with a(1)=3, a(2)=19. */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* number of slots currently allocated  (indices 0..cap-1) */

/* Ensure that cache can store indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_cap > (size_t)n) return;
    size_t new_cap = (cache_cap > 0) ? cache_cap : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) abort();          /* allocation failure */
    /* initialise new slots to zero */
    for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n<1 */

    ensure_cache(n);

    /* initialise base cases once */
    if (cache[1] == 0) cache[1] = 3;
    if (n >= 2 && cache[2] == 0) cache[2] = 19;

    if (n < cache_cap && cache[n] != 0) return cache[n];

    for (idx_t i = 3; i <= n; ++i) {
        if (cache[i] != 0) continue;    /* already computed */

        seq_t nn   = (seq_t)i;          /* n as seq_t */
        seq_t nn1  = (seq_t)(i - 1);    /* n-1 as seq_t */

        /* term1 = n * (18*n^2 + 21*n - 19) * a(n-1) */
        seq_t term1_mult = (seq_t)18 * nn * nn
                           + (seq_t)21 * nn
                           - (seq_t)19;
        seq_t term1 = nn * term1_mult * cache[i-1];

        /* term2 = 5*n*(3*n + 2)*(n-1)*a(n-2) */
        seq_t term2 = ((seq_t)5 * nn)
                      * ((seq_t)3 * nn + (seq_t)2)
                      * nn1
                      * cache[i-2];

        /* denominator = (n-1)*(3*n-1)*(n+4) */
        seq_t denom = nn1
                      * ((seq_t)3 * nn - (seq_t)1)
                      * (nn + (seq_t)4);

        seq_t numer = term1 - term2;    /* numerator is divisible by denom */
        cache[i] = numer / denom;
    }

    return cache[n];
}
