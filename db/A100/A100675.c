/*
 * A100675  a(1) = 1; for n >= 1, a(n+1) = Sum_{k=1..n} gcd(k, a(n)).
 * https://oeis.org/A100675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>=1, a(n+1)=sum_{k=1..n} gcd(k,a(n))
 * Range:     u8 n <= 49, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   local_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation caches computed terms for arbitrary-precision
 * seq_t and fast idx_t (uint64_t) indices. It works in both C11 and
 * C++17 as long as seq_t supports +, -, *, /, %, %% and gcd operations. */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/local_gcd.h"

/* Cached values: a(1) stored at cached_a[0] */
static seq_t *cached_a = NULL;
static size_t cached_len = 0;   /* number of values present            */
static size_t cached_cap = 0;   /* allocated capacity (elements)        */

/* Ensure at least `len` elements can be stored */
static void ensure_capacity(size_t len)
{
    if (len <= cached_cap)
        return;
    size_t new_cap = cached_cap ? cached_cap * 2 : 16;
    if (new_cap < len)
        new_cap = len;
    /* explicit cast for C++ compatibility */
    seq_t *tmp = (seq_t *)realloc(cached_a, new_cap * sizeof(seq_t));
    cached_a = tmp;
    cached_cap = new_cap;
}

/* The OEIS sequence A100675 */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n<1, but should never be called */

    /* Initialise a(1) on first call */
    if (cached_len == 0)
    {
        ensure_capacity(1);
        cached_a[0] = (seq_t)1;   /* a(1) = 1 */
        cached_len = 1;
    }

    /* If requested value already computed, return it directly */
    if ((size_t)n <= cached_len)
        return cached_a[n - 1];

    /* Compute missing terms up to n */
    ensure_capacity((size_t)n);
    for (idx_t cur = cached_len + 1; cur <= n; ++cur)
    {
        seq_t sum = 0;
        seq_t a_prev = cached_a[cur - 2];   /* a(cur-1) */
        for (idx_t k = 1; k < cur; ++k)
        {
            seq_t g = local_gcd((seq_t)k, a_prev);
            sum += g;
        }
        cached_a[cur - 1] = sum;
    }
    cached_len = (size_t)n;
    return cached_a[n - 1];
}
