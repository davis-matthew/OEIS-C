/*
 * A154484  a(n+1)-+a(n) = prime, a(1)=0, a(2)=2.
 * https://oeis.org/A154484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1869, u32 n <= 1870, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(2)=2.
   For n>2, a(n) is the smallest integer > a(n-1) such that
   a(n)-a(n-1) and a(n)+a(n-1) are both prime.  The function memoizes
   results so that calls with arbitrary n are fast and order‑independent. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;    /* 1‑based cache: cache[0] = a(1)                */
static size_t cap    = 0;      /* allocated capacity (in terms)                */
static idx_t  len    = 0;      /* number of precomputed terms                  */

/* Ensure cache has room for at least 'needed' elements.  Resizes via realloc. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cap) return;
    size_t newcap = cap ? cap : 8;
    while (newcap < needed) newcap <<= 1u;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) exit(1);                     /* abort on OOM                      */
    cache = tmp;
    cap   = newcap;
}

/* Initialise cache with the two known terms. */
static void init_cache(void)
{
    ensure_capacity(2);
    cache[0] = (seq_t)0;   /* a(1) */
    cache[1] = (seq_t)2;   /* a(2) */
    len = 2;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1, return 0 */

    if (len == 0) init_cache();          /* first call initializes base terms */

    /* Extend cache up to requested index, if necessary. */
    if (n > len)
    {
        for (idx_t i = len + 1; i <= n; ++i)
        {
            seq_t prev  = cache[i - 2];   /* a(i-1)                      */
            seq_t cand  = prev + (seq_t)1;/* start search at prev+1      */

            while (1)
            {
                seq_t diff = cand - prev;
                seq_t sum  = cand + prev;
                if (is_prime_big(diff) && is_prime_big(sum))
                    break;                /* found valid a(i)              */
                cand = cand + (seq_t)1;   /* try next candidate            */
            }
            ensure_capacity(i);           /* guarantee space for a(i)      */
            cache[i - 1] = cand;          /* store a(i) at index i-1       */
        }
        len = n;
    }

    return cache[n - 1];                  /* retrieve a(n)                  */
}
