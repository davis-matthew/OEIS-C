/*
 * A170301  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A170301 – Number of reduced words of length n in the
    Coxeter group with generators S_1,…,S_4 and relations
    (S_i)^2 = (S_i S_j)^4 = I for i≠j.
    The generating function is the rational function given in the
    OEIS entry.  From it we obtain a linear recurrence of order 42:

          a(n) = b(n) + 2·(a(n-1)+…+a(n-41))  – 3·a(n-42)   (n ≥ 0)

    where

          b(0)=1,  b(1)=…=b(41)=2,  b(42)=1,  b(n)=0 for n>42.

    This recurrence is used to compute all terms.  The function keeps
    a static cache so that repeated calls with arbitrary indices run in
    O(max(n)) time and O(max(n)) memory.  All arithmetic is performed
    in `seq_t`; the constants are cast to that type before the
    operations.  The code compiles under C11 and C++17. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* array of a(k) values           */
    static size_t cache_len = 0;    /* number of computed values      */
    static size_t cache_cap = 0;    /* allocated capacity             */

    /* Ensure we have enough capacity for a(n) */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 2 * cache_cap) new_cap = 2 * cache_cap;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed          */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first term if not already done */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;  /* a(0) = 1 */
        cache_len = 1;
    }

    /* If the requested term is already computed, return it */
    if ((size_t)n < cache_len) {
        return cache[n];
    }

    /* Compute sliding sum of the last up to 41 terms needed for the recurrence */
    seq_t sum41 = 0;
    size_t i_start = cache_len;
    if (i_start > 0) {
        size_t start = i_start - 1;
        size_t end   = (start >= 41 ? start - 41 : 0);
        for (size_t k = start; k >= end && k < (size_t)n + 1; --k) {
            sum41 += cache[k];
            if (k == end) break;
        }
    }

    /* Fill in terms from i_start up to n */
    for (size_t i = i_start; i <= (size_t)n; ++i) {
        /* b(i) from the numerator */
        seq_t b_i = 0;
        if (i == 0)           b_i = (seq_t)1;
        else if (i <= 41)     b_i = (seq_t)2;
        else if (i == 42)     b_i = (seq_t)1;
        /* otherwise b_i stays 0 */

        /* Recurrence: a(i) = b_i + 2*sum41 - 3*a(i-42) (if i>=42) */
        seq_t term = b_i + ((seq_t)2 * sum41);
        if (i >= 42) {
            term -= ((seq_t)3 * cache[i - 42]);
        }

        cache[i] = term;

        /* Update sliding sum for the next index (i+1) */
        sum41 += cache[i];
        if (i + 1 >= 42) {
            sum41 -= cache[(i + 1) - 42];
        }
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
