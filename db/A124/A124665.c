/*
 * A124665  Numbers that cannot be either prefixed or followed by one digit to
 *          form a prime.
 * https://oeis.org/A124665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 161, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A124665 – Numbers that cannot be either prefixed or followed
   by one digit to form a prime. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/isPrimeGeneric.h"
#include "../utils/pow10.h"

/* Memoised cache. */
static seq_t *cache = NULL;     /* array of found terms   */
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap  = 0;   /* current capacity       */

/* Grow the cache array to hold at least `needed` elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap * 2;
    if (new_cap == 0) new_cap = 8;
    while (new_cap < needed) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Return true if x satisfies the definition of A124665. */
static bool is_bad(seq_t x)
{
    /* x must not be prime. */
    if (isPrimeGeneric(x)) return false;

    /* Digit count of x. */
    seq_t tmp = x;
    int len = 0;
    do { len++; tmp /= 10; } while (tmp > 0);

    /* 10^len (used for prefix construction). */
    seq_t pow10k = pow10((idx_t)len);

    for (int d = 0; d <= 9; ++d)
    {
        /* Prefix: d*10^len + x.   For d==0 we get x itself. */
        seq_t pref = (seq_t)d * pow10k + x;
        if (isPrimeGeneric(pref)) return false;

        /* Suffix: x*10 + d. */
        seq_t suff = x * 10 + d;
        if (isPrimeGeneric(suff)) return false;
    }
    return true;
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for 0 */

    /* Ensure cache covers at least n terms. */
    while (cache_size < n)
    {
        /* Search for the next term. */
        static seq_t cur = 1;
        do { cur++; } while (!is_bad(cur));

        grow_cache(cache_size + 1);
        cache[cache_size++] = cur;
    }
    return cache[n - 1];
}
