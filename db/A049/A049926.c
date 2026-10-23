/*
 * A049926  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = n -
 *          1 - 2^p and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1 and a(2) = a(3) = 3.
 * https://oeis.org/A049926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n-1} a(k) - a(m) where m = n-1-2^p and p is
 *            the unique integer such that 2^p < n-1 <= 2^(p+1). Initial
 *            values: a(1)=1, a(2)=a(3)=3
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"


static seq_t *values = NULL;
static seq_t *sums   = NULL;
static size_t   capacity = 0;
static idx_t    max_computed = 0;

static void ensure_capacity(size_t needed)
{
    if (capacity < needed)
    {
        size_t new_cap = capacity ? capacity * 2 : 8;
        while (new_cap < needed)
            new_cap <<= 1;

        values = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
        sums   = (seq_t *)realloc(sums,   new_cap * sizeof(seq_t));
        capacity = new_cap;
    }
}

static idx_t compute_p(idx_t v) /* v = n-1, v >= 3 */
{
    idx_t p = floor_log2_uint64(v);      /* floor(log2(v)) */
    if ((idx_t)1 << p == v)              /* v is an exact power of two */
        p = (p > 0) ? p - 1 : 0;
    return p;
}

/* a(n) computable for any n>=1, memoised, any order of calls */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2 || n == 3) return (seq_t)3;

    /* initialise tables on first use */
    if (max_computed == 0)
    {
        ensure_capacity(4);
        values[0] = (seq_t)0;
        values[1] = (seq_t)1;
        values[2] = (seq_t)3;
        values[3] = (seq_t)3;
        sums[0]   = (seq_t)0;
        sums[1]   = values[1];
        sums[2]   = sums[1] + values[2];
        sums[3]   = sums[2] + values[3];
        max_computed = 3;
    }

    if (n <= max_computed)
        return values[n];

    ensure_capacity((size_t)n + 1);
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        idx_t v = i - 1;
        idx_t p = compute_p(v);
        idx_t pow2p = ((idx_t)1) << p;
        idx_t m = v - pow2p;            /* m = i-1-2^p, guaranteed >=1 */
        values[i] = sums[i-1] - values[m];
        sums[i]   = sums[i-1] + values[i];
    }

    max_computed = n;
    return values[n];
}
