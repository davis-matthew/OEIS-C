/*
 * A033641  Base 4 digital convolution sequence.
 * https://oeis.org/A033641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0) = 1. For n > 0, let the digits of n written in base 4 be d0
 *            (least significant), d1, …, d(k‑1) (most significant). Then a(n)
 *            = Σ_{i=0}^{k-1} d_i · a(n‑1‑i) (convolution with the digits).
 *            The function memoises all computed values so that any order of
 *            calls works
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#ifndef __cplusplus
/* C-only implementation using a dynamic array */
static seq_t *cache_arr = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
#endif

#ifdef __cplusplus
/* C++ implementation using std::vector (handles big‑int types) */
#include <vector>
static std::vector<seq_t> cache_vec;
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* Use a static vector and a static counter to remember the upper bound
       of computed entries. */
    static size_t computed = 0;               /* number of entries that are known */
    if ((idx_t)cache_vec.size() <= n)
        cache_vec.resize((size_t)n + 1, 0);   /* initialise new entries with 0 */

    for (idx_t k = (idx_t)computed; k <= n; ++k)
    {
        if (k == 0)            /* base case */
            cache_vec[0] = (seq_t)1;
        else
        {
            seq_t sum = 0;
            idx_t temp = k;
            idx_t shift = 1;
            while (temp > 0)
            {
                unsigned int coeff = (unsigned int)(temp & 3); /* digit in base 4 */
                sum += coeff * cache_vec[(size_t)(k - shift)];
                shift++;
                temp >>= 2;  /* divide by 4 */
            }
            cache_vec[(size_t)k] = sum;
        }
    }
    computed = (size_t)n + 1;
    return cache_vec[(size_t)n];
#else
    /* C implementation */
    static size_t computed = 0; /* number of entries already computed */
    if ((idx_t)computed <= n)
    {
        /* allocate (or reallocate) the array if necessary */
        if (n + 1 > cache_cap)
        {
            size_t new_cap = (size_t)(n + 1);
            seq_t *tmp = (seq_t *)realloc(cache_arr, new_cap * sizeof(seq_t));
            cache_arr = tmp;
            cache_cap = new_cap;
        }

        /* initialise new entries to 0 so that we can safely assign to them */
        for (size_t i = cache_len; i + 1 <= n; ++i)
            cache_arr[i] = 0;

        /* compute all required values */
        for (idx_t k = (idx_t)computed; k <= n; ++k)
        {
            if (k == 0)            /* base case */
                cache_arr[0] = (seq_t)1;
            else
            {
                seq_t sum = 0;
                idx_t temp = k;
                idx_t shift = 1;
                while (temp > 0)
                {
                    unsigned int coeff = (unsigned int)(temp & 3); /* digit in base 4 */
                    sum += coeff * cache_arr[(size_t)(k - shift)];
                    shift++;
                    temp >>= 2;  /* divide by 4 */
                }
                cache_arr[(size_t)k] = sum;
            }
        }
        computed = (size_t)n + 1;
        cache_len = computed; /* keep length in sync with computed */
    }
    return cache_arr[(size_t)n];
#endif
}
