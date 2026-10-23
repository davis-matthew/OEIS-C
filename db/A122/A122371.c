/*
 * A122371  Dimension of 7-variable non-commutative harmonics (twisted
 *          derivative). The dimension of the space of non-commutative
 *          polynomials in 7 variables which are killed by all symmetric
 *          differential operators (where for a monomial w, d_{xi} ( xi w ) =
 *          w and d_{xi} ( xj w ) = 0 for i/=j).
 * https://oeis.org/A122371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-1) - 170*a(n-2) + 669*a(n-3) - 1314*a(n-4) +
 *            1157*a(n-5) - 309*a(n-6) for n >= 6 Initial values: a(0)=1,
 *            a(1)=6, a(2)=41, a(3)=285, a(4)=1989, a(5)=13901
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term of OEIS A122371.  The function uses a
   static cache and computes terms only once, so subsequent
   calls are O(1).  Operations are restricted to integer
   arithmetic with +, -, and * as required. */
seq_t a(idx_t n)
{
    /* Static cache of computed values.  cache_size is the number
       of entries currently stored (>= n+1 after each growth). */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Base values for n = 0 .. 5.  These are known from the
       OEIS entry and required for the recurrence. */
    static const seq_t base[6] = { (seq_t)1, (seq_t)6, (seq_t)41,
                                   (seq_t)285, (seq_t)1989, (seq_t)13901 };

    /* Ensure we have room for indices 0 .. n inclusive. */
    size_t needed = (size_t)n + 1;
    if (needed > cache_size)
    {
        /* Allocate or reallocate the cache.  The cast is required
           for C11 and C++17 compatibility. */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp && needed > 0)
            return (seq_t)0;          /* allocation failed – unlikely */
        cache = tmp;
        /* Compute any missing entries. */
        size_t start = cache_size;
        if (start == 0) start = 0;
        for (size_t i = start; i < needed; ++i)
        {
            if (i < 6)
            {
                cache[i] = base[i];
            }
            else
            {
                /* Assemble the linear recurrence.  All intermediate
                   results fit within seq_t because the sequence grows
                   slowly for the small types used in the test suite. */
                seq_t pos = cache[i-1] * (seq_t)21;
                pos += cache[i-3] * (seq_t)669;
                pos += cache[i-5] * (seq_t)1157;

                seq_t neg = cache[i-2] * (seq_t)170;
                neg += cache[i-4] * (seq_t)1314;
                neg += cache[i-6] * (seq_t)309;

                cache[i] = pos - neg;
            }
        }
        cache_size = needed;
    }

    return cache[(size_t)n];
}
