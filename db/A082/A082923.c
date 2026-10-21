/*
 * A082923  Sort the digits of these triangular numbers into descending order
 *          and drop zeros to get primes.
 * https://oeis.org/A082923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th triangular number T_k such that sorting the
 *            decimal digits of T_k in descending order, dropping all zeros,
 *            yields a prime
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 19, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   digit_frequencies, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utility functions
   (headers are provided by the build system) */
#include <stddef.h>            /* size_t */
#include <stdbool.h>           /* bool */
#include <stdlib.h>            /* malloc, realloc */

#include "../utils/digit_frequencies.h"   /* digit_frequencies() */
#include "../utils/is_prime_big.h"        /* is_prime_big() */

/* seq_t, idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* Cache of the computed sequence values.  The cache is
       grown lazily on demand. */
    static seq_t *cache = NULL;            /* array of seq_t values */
    static size_t cache_size = 0;          /* number of values stored */
    static size_t cache_cap   = 0;          /* allocated capacity */

    /* Triangular numbers are generated incrementally. */
    static idx_t  cur_k = 1;               /* current k (1‑based) */
    static seq_t  cur_t = (seq_t)1;        /* T_1 = 1 */

    /* Generate terms until we have at least n of them. */
    while (cache_size < n)
    {
        /* Compute the number obtained by sorting the digits of cur_t
           in descending order and dropping zeros. */
        unsigned int freq[10] = {0};
        digit_frequencies(cur_t, freq);

        seq_t d = 0;
        for (int digit = 9; digit >= 1; --digit)
        {
            unsigned int cnt = freq[digit];
            for (unsigned int i = 0; i < cnt; ++i)
                d = d * 10 + digit;
        }

        /* If the resulting number is a prime, store the triangular
           number itself. */
        if (d != 0 && is_prime_big(d))
        {
            if (cache_size >= cache_cap)
            {
                size_t new_cap = (cache_cap == 0 ? 4 : 2 * cache_cap);
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();        /* allocation failed */
                cache   = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cur_t;
        }

        /* Advance to the next triangular number: T_{k+1} = T_k + (k+1). */
        ++cur_k;
        cur_t += (seq_t)cur_k;
    }

    /* a(1) corresponds to cache[0].  Return the requested term. */
    return cache[n - 1];
}
