/*
 * A101499  A Chebyshev transform of the Catalan numbers.
 * https://oeis.org/A101499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0..⌊n/2⌋} binomial(n-k,k) * Catalan(n-2k);
 *            Recurrence: (n+1)a(n) +
 *            2(-2n+1)a(n-1)+2(n-1)a(n-2)+2(-2n+3)a(n-3)+(n-3)a(n-4)=0 Initial
 *            values: a(0)=1, a(1)=1, a(2)=1, a(3)=3
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 71,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 972 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

static seq_t *cache = NULL;   /* array of computed terms */
static idx_t cache_size = 0;  /* number of valid entries in cache */

/* Ensure that cache contains terms up to and including n */
static void ensure_cache(idx_t n)
{
    if (cache_size == 0) {
        /* Allocate initial capacity for 4 terms and set them */
        cache_size = 4;
        cache = mem_new(seq_t, cache_size);
        if (!cache) abort();
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 3;
    }

    /* If we already have enough terms, nothing more to do */
    if (n < cache_size) return;

    /* Resize cache to hold up to n */
    idx_t new_size = n + 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_size);
    if (!tmp) abort();
    cache = tmp;

    /* Compute terms from cache_size up to n using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* Compute terms in the recurrence using unsigned arithmetic.
           All intermediate results are non‑negative for valid n.       */
        seq_t term1 = ((seq_t)(4 * i - 2)) * cache[i - 1];   /* (4i-2) * a_wide(i-1)   */
        seq_t term3 = ((seq_t)(4 * i - 6)) * cache[i - 3];   /* (4i-6) * a_wide(i-3)   */
        seq_t term2 = ((seq_t)(2 * i - 2)) * cache[i - 2];   /* (2i-2) * a_wide(i-2)   */
        seq_t term4 = ((seq_t)(i - 3)) * cache[i - 4];       /* (i-3) * a_wide(i-4)   */

        seq_t temp = term1 + term3;      /* term1 + term3   */
        temp -= term2;                   /* - term2         */
        temp -= term4;                   /* - term4         */

        cache[i] = temp / ((seq_t)(i + 1));  /* divide by (i+1) */
    }
    cache_size = new_size;
}

seq_t a_wide(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
