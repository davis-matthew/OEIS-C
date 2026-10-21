/*
 * A235400  a(1)=1. For n>1, assume a(n-1) has decimal expansion qrstuvwxyz
 *          (with at most ten digits, where some of q, r, s, ... may be zero).
 *          Then a(n) = sum 99...9 (with z 9's) + 88...8 (with y 8's) + 77...7
 *          (with x 7's) + ... For example, if a(n-1) were 243, we would sum
 *          77 + 8888 + 999 and get 9964.
 * https://oeis.org/A235400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(1)=1; for n>1, if a(n-1) has decimal digits d_0…d_k (d_0 =
 *            units) then a(n)=Σ_{i=0}^{k-1} (9-i) * (10^{d_i} – 1) / 9 where
 *            the sum is over all digits of a(n-1). Digits beyond the 10th are
 *            ignored (but they never occur in the sequence)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow10.h"

static seq_t *cache = NULL;          /* stored terms, 0‑based (a(1) at cache[0]) */
static idx_t cache_cap = 0;          /* capacity of cache array */
static idx_t cache_len = 0;          /* number of terms currently stored */

/* compute next term from previous term */
static seq_t compute_next(seq_t prev)
{
    seq_t result = (seq_t)0;
    seq_t tmp = prev;
    idx_t i = 0;

    /* extract digits from least significant upwards,
       stop after 10 digits as per the definition            */
    while (tmp > (seq_t)0 && i < 10)
    {
        seq_t d = tmp % (seq_t)10;          /* current digit (0–9) */
        tmp = tmp / (seq_t)10;              /* remove the digit */
        if (d != (seq_t)0)
        {
            idx_t exp = (idx_t)d;           /* number of repetitions */
            seq_t powten = pow10(exp);      /* 10^d   */
            seq_t repunit = (powten - (seq_t)1) / (seq_t)9; /* d copies of 1 */
            seq_t term = repunit * ((seq_t)(9 - i));        /* place value */
            result += term;
        }
        ++i;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0; /* undefined for n<1 */

    /* ensure cache is seeded with a(1)=1 */
    if (cache_len == (idx_t)0)
    {
        cache_cap = (idx_t)1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;
        cache_len = (idx_t)1;
    }

    /* grow cache if needed and compute missing terms */
    if (cache_len < n)
    {
        if (n > cache_cap)
        {
            cache_cap = n;
            seq_t *new_cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            if (!new_cache) return (seq_t)0;   /* allocation failure */
            cache = new_cache;
        }
        for (idx_t i = cache_len; i < n; ++i)
        {
            cache[i] = compute_next(cache[i-1]);
        }
        cache_len = n;
    }

    return cache[n-1];  /* 0‑based index to get a(n) */
}
