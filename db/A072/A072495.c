/*
 * A072495  Automorphic numbers: numbers k such that k^21 ends with k. Also
 *          m-morphic numbers for any m such that (m-1)/10 is an even integer
 *          not divisible by 10.
 * https://oeis.org/A072495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Compute the nth 21‑morphic (automorphic) number: the sequence of
 *            non‑negative integers k for which k^21 ≡ k (mod 10^digits(k)).
 *            a(1)=0, a(2)=1, …
 * Range:     u8 n <= 66, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   mod_pow, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Search for the numbers in increasing order, caching the found
    terms so that subsequent calls are fast.  All arithmetic uses the
    utilities supplied by the build, in particular pow10 for 10^d and
    mod_pow for modular exponentiation.  The digit count is computed
    locally, avoiding any missing utility.  */

#include <stdlib.h>        /* malloc, realloc */
#include "../utils/mod_pow.h"  /* mod_pow */
#include "../utils/pow10.h"    /* pow10 */

/* Compute the number of decimal digits of an unsigned 64‑bit integer.
   The function works for all idx_t values that fit in 64 bits and
   returns 1 for 0.  */
static idx_t digit_count(idx_t x)
{
    idx_t d = 1;
    while (x >= 10)
    {
        x /= 10;
        ++d;
    }
    return d;
}

seq_t a(idx_t n)
{
    /* Static cache of already discovered 21‑morphic numbers. */
    static seq_t *cache = NULL;    /* array of results         */
    static size_t cache_len = 0;   /* number of elements stored*/
    static size_t cache_cap = 0;   /* allocated capacity       */
    static idx_t next_candidate = 0; /* integer to test next      */

    if (n == 0)  /* undefined for offset 1, return 0 */
        return (seq_t)0;

    /* Extend cache until it holds at least n terms.  */
    while (cache_len < (size_t)n)
    {
        seq_t cand = (seq_t)next_candidate;
        idx_t  d   = digit_count(next_candidate);
        seq_t  mod = pow10(d);             /* 10^d, safe for 64‑bit seq_t up to d ≈ 19 */
        seq_t  power = mod_pow(cand, 21, mod);

        if (power == cand)
        {
            /* Grow the array if necessary.  */
            if (cache_len >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)    /* allocation failure – return 0 (no further info) */
                    return (seq_t)0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
