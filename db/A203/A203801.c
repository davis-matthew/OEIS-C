/*
 * A203801  G.f.: Product_{n>=1} (1 + Lucas(n)*x^n + (-1)^n*x^(2*n)) where
 *          Lucas(n) = A000204(n).
 * https://oeis.org/A203801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  A203801 sequence.
 *  Recurrence (OEIS definition):
 *      a(0) = 1
 *      a(n) = (1/n) * Σ_{k=1..n} A000593(k) * Lucas(k) * a(n-k)    (n>0)
 *  where:
 *      Lucas(k)   = Lucas numbers (L(0)=2, L(1)=1, L(k)=L(k-1)+L(k-2))
 *      A000593(k) = sum of odd divisors of k
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* External linkage required by the framework. */
seq_t a(idx_t n)
{
    /* Static caches for computed values, growing lazily. */
    static seq_t *a_cache   = NULL;   /* a values               */
    static seq_t *luc_cache = NULL;   /* Lucas numbers          */
    static seq_t *odd_cache = NULL;   /* sum of odd divisors    */
    static size_t cache_size = 0;     /* number of entries stored (size_t for array index) */

    /* Target size (n + 1 because index 0 is included). */
    size_t target = (size_t)n + 1;

    /* Expand cache if required. */
    if (target > cache_size)
    {
        /* New size */
        size_t new_size = target;

        /* Allocate or reallocate arrays */
        /* a_cache, luc_cache, odd_cache may be NULL on first call */
        a_cache   = (seq_t *)realloc(a_cache,   new_size * sizeof(seq_t));
        luc_cache = (seq_t *)realloc(luc_cache, new_size * sizeof(seq_t));
        odd_cache = (seq_t *)realloc(odd_cache, new_size * sizeof(seq_t));

        /* Initialise new entries for odd divisor sums */
        for (size_t i = cache_size; i < new_size; ++i)
            odd_cache[i] = (seq_t)0;

        /* Update odd divisor sums for the new range.
           Incrementally add contributions of all odd divisors d
           to multiples m ∈ [cache_size, new_size-1]. */
        for (size_t d = 1; d < new_size; d += 2)
        {
            /* First multiple of d that lies in the new interval. */
            size_t start = ((cache_size + d - 1) / d) * d;
            if (start < cache_size) start = d;   /* safety, should not happen */
            for (size_t m = start; m < new_size; m += d)
                odd_cache[m] += (seq_t)d;
        }

        /* Update Lucas numbers for the new range. */
        if (cache_size == 0)
        {
            /* Initialize base Lucas numbers. */
            luc_cache[0] = (seq_t)2;
            if (new_size > 1)
                luc_cache[1] = (seq_t)1;
            /* Base a[0] */
            a_cache[0] = (seq_t)1;
        }
        else
        {
            /* Ensure luc[0] and luc[1] are set if newly added. */
            if (new_size > 0 && cache_size <= 0)
                luc_cache[0] = (seq_t)2;
            if (new_size > 1 && cache_size <= 1)
                luc_cache[1] = (seq_t)1;
        }

        for (size_t i = cache_size; i < new_size; ++i)
        {
            if (i == 0) continue;              /* already set */
            if (i == 1) continue;              /* already set */
            luc_cache[i] = luc_cache[i-1] + luc_cache[i-2];
        }

        /* Compute a(n) for new indices using the convolution recurrence. */
        for (size_t i = cache_size; i < new_size; ++i)
        {
            if (i == 0) continue; /* a[0] already known */
            unsigned __int128 sum = 0;
            for (size_t k = 1; k <= i; ++k)
            {
                sum += (unsigned __int128)odd_cache[k]
                        * (unsigned __int128)luc_cache[k]
                        * (unsigned __int128)a_cache[i - k];
            }
            a_cache[i] = (seq_t)(sum / (unsigned __int128)i);
        }

        cache_size = new_size;
    }

    /* Return the requested term. */
    return a_cache[(size_t)n];
}
