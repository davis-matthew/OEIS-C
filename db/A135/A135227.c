/*
 * A135227  Triangle A000012 * A135225, read by rows.
 * https://oeis.org/A135227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r+1 if n is the first index of row r, * otherwise a(n) =
 *            binomial(r, n - r*(r+1)/2), * where r is the unique integer with
 *            r*(r+1)/2 <= n < (r+1)*(r+2)/2
 * Range:     u8 n <= 69, u16 n <= 197, u32 n <= 646, u64 n <= 2376,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute binomial(n,k) with k <= n.  Works for all seq_t types. */
static seq_t binomial_k(idx_t n, idx_t k)
{
    if (k == 0 || k == n)
        return (seq_t)1;
    if (k > n - k)
        k = n - k;
    if (k == 0)        /* k==0 after reduction */
        return (seq_t)1;

    size_t sz = (size_t)k;
    /* Array of remaining denominators. */
    idx_t *den = (idx_t *)malloc(sz * sizeof(idx_t));
    if (!den)
        return (seq_t)0; /* allocation failure – unlikely for small k */

    for (size_t i = 0; i < sz; ++i)
        den[i] = (idx_t)(i + 1);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < sz; ++i)
    {
        idx_t num = n - sz + i + 1;  /* numerator part of binomial */
        /* Reduce num against all pending denominators. */
        for (size_t j = 0; j < sz; ++j)
        {
            if (den[j] == 1)
                continue;
            idx_t g = gcd_idx(num, den[j]);
            if (g > 1)
            {
                num /= g;
                den[j] /= g;
                if (den[j] == 1)
                    continue; /* skip further divisions for this denom */
            }
        }
        /* Multiply by the reduced numerator. */
        if (num > 1)
            result = result * (seq_t)num;
    }
    free(den);
    return result;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t lo = 0, hi = n, r = 0;
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1);
        idx_t tri = (idx_t)(t / 2);
        if (tri <= n)
        {
            r = mid;
            lo = mid + 1;
        }
        else
        {
            if (mid == 0)
                break; /* avoid underflow when hi becomes -1 */
            hi = mid - 1;
        }
    }

    /* Start index of this row. */
    unsigned __int128 t_start = (unsigned __int128)r * (r + 1);
    idx_t row_start = (idx_t)(t_start / 2);

    idx_t offset = n - row_start;

    if (offset == 0)
        return (seq_t)(r + 1);     /* left‑border special value */

    return binomial_k(r, offset);
}
