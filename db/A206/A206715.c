/*
 * A206715  Numbers matched to polynomials divisible by x^2+1.
 * https://oeis.org/A206715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th positive integer whose binary
 * representation viewed as a polynomial with coefficients in {0,1}
 * is divisible by x^2+1.  The numbers are enumerated by interpreting
 * the integer’s binary digits as the coefficients of the polynomial.
 *
 * The test for divisibility uses the fact that p(i)=0 for i^2 = -1
 * iff
 *   Σ_{even j} a_j (-1)^(j/2) = 0  and  Σ_{odd j} a_j (-1)^((j-1)/2) = 0
 * where a_j is the j-th binary digit of the integer.
 *
 * The sequence is generated lazily: the routine keeps a static
 * list of found values and extends it until the requested term is
 * available.  This satisfies all ordering constraints and keeps
 * the runtime below the required limits for n up to about 10 000.
 */

#include <stdlib.h>
#include <stdbool.h>

static inline bool good(uint64_t n)
{
    int evenSum = 0, oddSum = 0;
    unsigned pos = 0;

    while (n)
    {
        if (n & 1U)
        {
            if ((pos & 1U) == 0U) /* even position */
                evenSum += ((pos >> 1) & 1U) ? -1 : 1;
            else                 /* odd position */
                oddSum  += ((pos >> 1) & 1U) ? -1 : 1;
        }
        pos++;
        n >>= 1U;
    }
    return evenSum == 0 && oddSum == 0;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t candidate = 1U;   /* next integer to test */

    while (cache_len < n)
    {
        /* Find the next good integer. */
        while (!good(candidate))
            ++candidate;

        /* Grow the cache if necessary. */
        if (cache_cap == 0)
        {
            cache_cap = 16;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        }
        else if (cache_len == cache_cap)
        {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        cache[cache_len++] = (seq_t)candidate;
        ++candidate;
    }

    return cache[n - 1];
}
