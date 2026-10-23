/*
 * A117799  Icosagonal numbers for which the sum of the digits is also an
 *          icosagonal number.
 * https://oeis.org/A117799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   digit_sum, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the nth icosagonal number k*(9k‑8) whose digit sum is also
    an icosagonal number.
    icosagonal numbers:  P_k = k*(9k-8)   (k = 0,1,2,…)
    condition: digit_sum(P_k) = t*(9t-8)  for some integer t ≥ 0.  */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/floor_sqrt.h"

/* test whether x is an icosagonal number */
static bool is_icosagonal(seq_t x)
{
    if (x == (seq_t)0)          /* 0 = 0*(9·0‑8) is icosagonal */
        return true;

    /* Solve 9t²‑8t‑x = 0 → D = 64 + 36x must be a perfect square.
       Let r = √D.  Then t = (8+r)/18, so r ≡ 10 (mod 18). */
    seq_t val = (seq_t)36 * x + (seq_t)64;
    seq_t r   = floor_sqrt(val);
    if (r * r != val)          /* not a perfect square */
        return false;

    /* check (r+8) is divisible by 18 */
    return ((r + (seq_t)8) % (seq_t)18) == (seq_t)0;
}

/*  a(n) with memoisation (indices not presumed increasing) */
static seq_t  *cache   = NULL;   /* array of found icosagonal numbers */
static size_t cache_cap = 0;     /* current array capacity            */
static size_t cache_len = 0;     /* number of stored elements         */
static idx_t  last_k   = 0;     /* last k processed (icos index)     */

seq_t a(idx_t n)
{
    /* initialise cache with the first term (k = 0) */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* 0 is a valid icosagonal number and its digit sum (0) is also icosagonal */
        cache[cache_len++] = (seq_t)0;
        last_k = 0;                                 /* k corresponding to the last processed value */
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        idx_t k = last_k + 1;                                /* next icos index */
        seq_t ico = ((seq_t)k) * (((seq_t)9 * k) - (seq_t)8);  /* k*(9k‑8) */

        if (is_icosagonal(digit_sum(ico))) {                 /* satisfies condition */
            if (cache_len == cache_cap) {                    /* grow array */
                size_t new_cap = cache_cap * 2;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len++] = ico;
        }

        last_k = k;
    }

    return cache[n - 1];   /* indices are 1‑based in OEIS */
}
