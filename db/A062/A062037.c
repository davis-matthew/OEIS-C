/*
 * A062037  Positive numbers whose product of digits is 6 times their sum.
 * https://oeis.org/A062037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th smallest positive integer x * such that the
 *            product of the decimal digits of x equals * 6 times the sum of
 *            its digits. The sequence is indexed * from 1. The algorithm
 *            simply tests consecutive integers * for the property and stores
 *            the ones that satisfy it. * Results are memoised s...
 * Range:     u8 none, u16 n <= 244, u32 n <= 500, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Helper: true if num satisfies P = 6 * S */
static bool satisfies(idx_t num)
{
    seq_t prod = 1;               /* product of digits */
    seq_t sum  = 0;               /* sum of digits */

    for (idx_t x = num; x > 0; x /= 10)
    {
        unsigned d = (unsigned)(x % 10);
        if (d == 0)               /* zero digit -> product 0, sum >0 => impossible */
            return false;
        sum  += (seq_t)d;
        prod *= (seq_t)d;
    }
    return prod == 6 * sum;
}

/*  Main function */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL; /* memoised sequence values      */
    static idx_t  cache_sz   = 0;    /* how many values are stored   */
    static idx_t  capacity   = 0;    /* allocated capacity           */
    static idx_t  next       = 1;    /* next integer to test         */

    /* grow cache if needed */
    if (cache_sz >= capacity)
    {
        capacity = capacity < 128 ? 128 : capacity * 2;
        cache    = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (cache_sz < n)
    {
        if (satisfies(next))
        {
            if (cache_sz >= capacity)      /* ensure space for new term */
            {
                capacity *= 2;
                cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
            }
            cache[cache_sz++] = next;
        }
        ++next;
    }

    return cache[n - 1];
}
