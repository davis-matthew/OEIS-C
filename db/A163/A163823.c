/*
 * A163823  Number of initial segments of signature sequences of length n.
 * https://oeis.org/A163823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + sum_{k=2..n} A163822(k) where A163822(k) = #{ (a,b) |
 *            a,b>=1, gcd(a,b)=1, (a+1)(b+1)=2k } (ordered)
 * Range:     u8 n <= 59, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Compute A163822 for a given n (n >= 2) */
static seq_t A163822(idx_t n)
{
    idx_t m = n * 2;          /* m fits in idx_t for all expected n */
    seq_t count = 0;
    for (idx_t d = 2; d * d <= m; ++d)
    {
        if (m % d == 0)
        {
            idx_t e = m / d; /* e >= 2 because d >= 2 */
            seq_t a = (seq_t)d - 1;
            seq_t b = (seq_t)e - 1;
            if (gcd(a, b) == 1)
            {
                if (d == e)
                    count += 1;          /* a == b case */
                else
                    count += 2;          /* (d,e) and (e,d) */
            }
        }
    }
    return count;
}

/* Memoized array of computed a(n) values */
static seq_t *a_cache = NULL;
static idx_t a_cache_cap = 0;
static idx_t a_computed = 0;   /* number of values stored (a(1) at index 0) */
static seq_t a_cumulative = 0; /* sum_{k=2..computed} A163822(k) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but guard against misuse */

    /* ensure cache exists and has enough capacity */
    if (!a_cache)
    {
        a_cache_cap = 64;
        a_cache = (seq_t*)malloc(a_cache_cap * sizeof(seq_t));
    }
    while (a_cache_cap < n)
    {
        a_cache_cap *= 2;
        a_cache = (seq_t*)realloc(a_cache, a_cache_cap * sizeof(seq_t));
    }

    /* compute values up to n if not already computed */
    while (a_computed < n)
    {
        idx_t next = a_computed + 1;   /* 1‑based index */
        seq_t add = 0;
        if (next >= 2)
            add = A163822(next);
        a_cumulative += add;          /* now cumulative sum up to 'next' */
        seq_t an = (next == 1) ? (seq_t)1 : ((seq_t)1 + a_cumulative);
        a_cache[a_computed] = an;
        ++a_computed;
    }

    return a_cache[n - 1];
}
