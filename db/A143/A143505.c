/*
 * A143505  Triangle of coefficients of the polynomials x^(n - 1)*A(n,x +
 *          1/x), where A(n,x) are the Eulerian polynomials of A008292.
 * https://oeis.org/A143505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n-th entry of the flattened triangle of coefficients
 *            of the polynomials x^(k-1)*A(k,x+1/x), where A(k,x) are the
 *            Eulerian polynomials. Row sums are A000670 without the leading 1
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root of a 64‑bit unsigned integer.
   Returns floor(sqrt(x)). Uses 128‑bit arithmetic to avoid overflow. */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t r = 0;
    for (int i = 63; i >= 0; --i)
    {
        uint64_t t = r | ((uint64_t)1 << i);
        if ((unsigned __int128)t * t <= x)
            r = t;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset is 1, defensive guard */

    /* Determine the row r such that (r-1)^2 < n ≤ r^2. */
    uint64_t r = isqrt_u64(n - 1) + 1;   /* r ≥ 1 */

    /* Position inside the row, 1‑based. */
    uint64_t pos = n - (r - 1) * (r - 1); /* 1 … 2r‑1 */

    /* Allocate two buffers for the Eulerian recurrence. */
    seq_t *prev = (seq_t *)calloc(r, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(r, sizeof(seq_t));
    if (!prev || !curr)
    {
        free(prev);
        free(curr);
        return 0;
    }

    /* Base row A(1,0) = 1. */
    prev[0] = 1;

    /* Build rows 2 … r. */
    for (uint64_t cur = 2; cur <= r; ++cur)
    {
        for (uint64_t k = 0; k < cur; ++k)
        {
            seq_t val = 0;
            if (k < cur - 1)
                val += prev[k] * (seq_t)(k + 1);
            if (k > 0)
                val += prev[k - 1] * (seq_t)(cur - k);
            curr[k] = val;
        }
        /* Swap buffers for next iteration. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Now prev[0..r-1] contains the Eulerian numbers A(r,k). */

    /* Index m (0‑based exponent) corresponding to position pos. */
    uint64_t m = pos - 1;

    /* Compute the requested coefficient as the sum over k and j. */
    seq_t result = 0;
    for (uint64_t k = 0; k < r; ++k)
    {
        int64_t diff = (int64_t)m - (int64_t)(r - 1) + (int64_t)k;
        if (diff < 0 || (diff & 1LL))
            continue;                      /* j would be negative or non‑integer */
        uint64_t j = (uint64_t)(diff >> 1);
        if (j > k)
            continue;

        /* Compute binomial coefficient C(k,j). */
        seq_t bin = 1;
        for (uint64_t i = 1; i <= j; ++i)
        {
            bin = bin * (seq_t)(k - j + i);
            bin = bin / (seq_t)i;          /* exact division */
        }

        result += prev[k] * bin;
    }

    free(prev);
    free(curr);
    return result;
}
