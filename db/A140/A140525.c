/*
 * A140525  a(1)=2. For n >=2, a(n) = the least integer >= a(n-1) that is not
 *          coprime to both a(n-1)+1 and a(n-1).
 * https://oeis.org/A140525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2 For n >= 2, a(n) = a(n-1) + 1 + lpf(a(n-1) + 1) (where
 *            lpf(k) is the least prime factor of k)
 * Range:     u8 n <= 12, u16 n <= 36, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Least prime factor of an unsigned 64‑bit integer.  It is used
   after casting the sequence value to uint64_t, which is safe
   for all terms up to n ≈ 10000 (they fit in 64 bits). */
static uint64_t lpf_uint64(uint64_t n)
{
    if (n <= 1) return n;
    if ((n & 1ULL) == 0) return 2ULL;
    for (uint64_t p = 3; p <= n / p; p += 2)
    {
        if (n % p == 0) return p;
    }
    return n; /* n itself is prime */
}

/* Cache of computed terms: cache[0] = a(1), cache[1] = a(2), ... */
static seq_t *cache        = NULL; /* array of seq_t values */
static size_t cache_len    = 0;    /* number of elements currently computed */

/* Ensure that a(1)..a(n) are computed */
static void ensure_up_to(idx_t n)
{
    if (n == 0) return;            /* undefined, nothing to compute */
    if ((idx_t)cache_len >= n)     /* already computed up to n */
        return;

    /* Grow the array if necessary */
    if (cache_len == 0)            /* first allocation */
    {
        /* Reserve space for at least n terms */
        size_t initial_cap = n > 32 ? n : 32;
        cache = (seq_t *)realloc(cache, initial_cap * sizeof(seq_t));
    }
    else if (n > cache_len)        /* increase capacity */
    {
        size_t new_cap = cache_len * 2;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    }

    /* If this is the very first call, set a(1) */
    if (cache_len == 0)
    {
        cache[0] = (seq_t)2;
        cache_len = 1;
    }

    /* Compute remaining terms up to n */
    for (size_t i = cache_len; i < (size_t)n; ++i)
    {
        seq_t prev = cache[i - 1];
        /* Convert prev+1 to uint64_t; it is safe for all terms of interest */
        uint64_t prev_plus1 = (uint64_t)(prev + (seq_t)1);
        uint64_t lpf = lpf_uint64(prev_plus1);
        seq_t step  = (seq_t)(lpf + 1ULL);   /* 1 + lpf(a(n-1)+1) */
        cache[i] = prev + step;
    }

    cache_len = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n < 1 */
    ensure_up_to(n);
    return cache[(size_t)n - 1];
}
