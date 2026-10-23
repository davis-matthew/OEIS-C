/*
 * A108722  a(1)=11, If a(n-1)=abcde..., where a,b,c,d,e... are the digits,
 *          then a(n)=abcde...+a*bcde...+ab*cde...+abc*de...+abcd*e...+....
 * https://oeis.org/A108722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A108722:
   a(1) = 11
   a(n) = a(n-1) + Σ_{i=1}^{d-1} (prefix_i * suffix_i)
   where d = number of decimal digits of a(n-1),
   prefix_i = first i digits of a(n-1),
   suffix_i = remaining d-i digits.
   This follows the definition in the OEIS entry. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_len.h"

#define CACHE_MAX 100001          /* cache up to 100000 terms */
static seq_t cache[CACHE_MAX];
static idx_t cache_len = 0;      /* last index computed and stored */

/* compute a(k) assuming a(k-1) is already in cache[k-1] */
static seq_t compute_next(idx_t k)
{
    seq_t prev = cache[k - 1];
    idx_t d = digit_len(prev);   /* number of decimal digits */

    /* powers of 10 up to 10^d stored in a local array */
    static seq_t pow10[50];
    pow10[0] = (seq_t)1;
    for (idx_t i = 1; i <= d; ++i)
        pow10[i] = pow10[i - 1] * (seq_t)10;

    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i < d; ++i) {   /* split after i digits */
        seq_t p10 = pow10[d - i];       /* 10^(d-i) */
        seq_t left  = prev / p10;        /* first i digits */
        seq_t right = prev % p10;        /* last d-i digits */
        sum += left * right;
    }
    return prev + sum;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* behavior below offset is undefined */

    /* initialise cache on first call */
    if (cache_len == 0) {
        cache[1] = (seq_t)11;
        cache_len = 1;
    }

    if (n <= cache_len)                   /* already known */
        return cache[n];

    /* compute further terms up to n, storing them if within CACHE_MAX */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        seq_t val = compute_next(k);
        if (k < CACHE_MAX)
            cache[k] = val;
    }
    if (n < CACHE_MAX)
        cache_len = n;                     /* update cache length if stored */

    /* return the required term (computed above) */
    return cache[n];
}
