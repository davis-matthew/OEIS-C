/*
 * A231309  Recurrence a(n) = a(n-2) + n^M for M=10, starting with a(0)=0,
 *          a(1)=1.
 * https://oeis.org/A231309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} (n-2*k)^10 Recurrence: a(n) = a(n-2) +
 *            n^10, with a(0) = 0 and a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 73, u128 n <= 4215,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Compute i^10 using repeated multiplication. */
static seq_t pow10_of(idx_t i)
{
    seq_t base = (seq_t)i;
    seq_t result = base;
    for (int k = 1; k < 10; ++k)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    /* Initialise base cases if necessary. */
    if (cache_len == 0)
    {
        cache_len = 2;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* Allocation failure. */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    idx_t prev_len = cache_len;                 /* Length before possible resize. */

    /* Grow cache to accommodate index n. */
    if (n + 1 > cache_len)
    {
        size_t new_len = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;              /* Allocation failure. */
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute missing terms by the recurrence. */
    idx_t start = (prev_len > 1) ? prev_len : 2;   /* Start from the first uncomputed index. */
    for (idx_t i = start; i <= n; ++i)
    {
        seq_t term = pow10_of(i);
        cache[i] = cache[i - 2] + term;
    }

    return cache[n];
}
