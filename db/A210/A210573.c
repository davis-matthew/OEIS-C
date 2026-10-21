/*
 * A210573  E.g.f.: (1-x^2)^(-3/2)*exp(-x^2+x/(1-x)).
 * https://oeis.org/A210573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A210573:  a(0)=1
   For n≥0:  a(n+1) = Σ_{k=0}^{n} r_k · a(n–k) · P(n,k)
   where  P(n,k)=n·(n–1)·…·(n–k+1)   (the falling factorial),
          r_0=1,
          r_1=r_2=3,
          r_k=k+1  for even k≥3,
          r_k=k+4  for odd  k≥3.
   (This recurrence follows from A'(x)=A(x)*(3x/(1−x²)−2x+1/(1−x)²).) */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* dynamic cache for computed values */
static seq_t *cache = NULL;          /* array of a(n) values */
static size_t cache_len = 0;         /* number of computed entries (0..len-1) */
static size_t cache_cap = 0;         /* allocated capacity */

/* ensure the cache can hold indices up to n (inclusive) */
static void ensure_cache_cap(idx_t n)
{
    size_t req = (size_t)n + 1;
    if (req > cache_cap)
    {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        if (new_cap < req) new_cap = req;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);           /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < cache_len) return cache[(size_t)n];  /* already computed */

    ensure_cache_cap(n);

    /* compute missing values up to n */
    for (size_t idx = cache_len; idx <= (size_t)n; ++idx)
    {
        if (idx == 0) {                /* base case a(0)=1 */
            cache[0] = (seq_t)1;
            continue;
        }

        /* recurrence: a(idx) = Σ_{k=0}^{idx-1} r_k * a[idx-1-k] * P(idx-1,k) */
        seq_t sum = (seq_t)0;
        seq_t P   = (seq_t)1;                     /* falling factorial */
        size_t prev = idx - 1;

        for (size_t k = 0; k <= prev; ++k)
        {
            /* compute r_k */
            seq_t r_k;
            if (k == 0)          r_k = (seq_t)1;
            else if (k == 1 || k == 2)  r_k = (seq_t)3;
            else if (k & 1ULL)   r_k = (seq_t)(k + 4);
            else                 r_k = (seq_t)(k + 1);

            seq_t term = cache[prev - k];
            term = term * r_k;
            term = term * P;
            sum = sum + term;

            if (k < prev)
                P = P * (seq_t)(prev - k);
        }
        cache[idx] = sum;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
