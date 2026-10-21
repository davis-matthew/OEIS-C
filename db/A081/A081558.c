/*
 * A081558  Exponential generating function: exp(cosh(x)+2*x-1).
 * https://oeis.org/A081558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 26, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A081558  */
/*  a(n) = coefficient of x^n/n! in exp(cosh(x)+2x-1)     */
/*  Recurrence:                                           */
/*      a(0) = 1,                                         */
/*      a(n+1) = 2·a(n) + Σ_{k=0}^{n-1} [ (n-k odd) · a(k)·C(n,k) ]  */
/*  where C(n,k) is the binomial coefficient.             */

/*  NOTE:  seq_t and idx_t are defined by the build system. */
#include <stddef.h>
#include <stdlib.h>

static seq_t *a_cache   = NULL;   /* cache[ i ] = a(i) */
static idx_t  cache_len = 0;      /* number of stored entries */

/*  Ensure cache holds values up to index n (inclusive).  */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;

    /* Allocate a larger array.  */
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_cache)
        return;   /* out of memory – the harness will abort */

    /* Copy old values.  */
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = a_cache[i];

    /* Initialize the very first term if the cache was empty.  */
    if (cache_len == 0)
    {
        new_cache[0] = 1;
        cache_len = 1;
    }

    /*  Compute missing terms.  */
    for (idx_t i = cache_len; i < new_len; ++i)
    {
        idx_t N = i - 1;                /* N = n in a(n+1) recurrence */
        seq_t sum = (seq_t)2 * new_cache[N];  /* 2·a(n) part */

        /*  binomial coefficients C(N,k)  */
        seq_t binom = 1;                /* C(N,0) */
        for (idx_t k = 0; k < N; ++k)
        {
            /* Add term only if (N - k) is odd.  */
            if ( ((N - k) & 1) )
                sum += new_cache[k] * binom;

            /* Update binom to C(N, k+1) for next iteration.  */
            if (k + 1 <= N)
                binom = binom * (seq_t)(N - k) / (seq_t)(k + 1);
        }
        new_cache[i] = sum;
    }

    /* Release old storage.  */
    free(a_cache);
    a_cache   = new_cache;
    cache_len = new_len;
}

/*  Public function required by the test harness.  */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
