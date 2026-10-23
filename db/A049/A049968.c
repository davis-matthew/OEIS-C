/*
 * A049968  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = 2*n
 *          - 3 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1, a(2) = 3, and a(3) = 2.
 * https://oeis.org/A049968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=3, a(3)=2; for n>=4, a(n)= (sum_{i=1}^{n-1} a(i)) +
 *            a(m) where m = 2*n-3-2^(p+1) and p is the unique integer with
 *            2^p < n-1 <= 2^(p+1) (i.e. p = floor(log2(n-2)) )
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

static idx_t floor_log2_idx(idx_t v)
{
    /* v >= 1 */
    idx_t r = 0;
    while (v >> 1) { v >>= 1; ++r; }
    return r;
}

seq_t a(idx_t n)
{
    /* Do nothing for n outside the sequence */
    if (n == 0) return (seq_t)0;

    /* Static cache of computed terms and their prefix sums */
    static seq_t *values = NULL;      /* a(1) .. a(max_n) */
    static seq_t *prefix = NULL;      /* prefix[i] = sum_{k=1}^i a(k) */
    static idx_t max_n  = 0;          /* highest n computed so far */

    /* Allocate or grow the cached arrays if necessary */
    if (n > max_n)
    {
        /* Grow values array */
        seq_t *new_values = (seq_t *)realloc(values, n * sizeof(seq_t));
        if (!new_values) return (seq_t)0;
        values = new_values;

        /* Grow prefix array */
        seq_t *new_prefix = (seq_t *)realloc(prefix, (n + 1) * sizeof(seq_t));
        if (!new_prefix) return (seq_t)0;
        prefix = new_prefix;

        /* Initialise prefix[0] */
        if (max_n == 0) prefix[0] = 0;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_n + 1; i <= n; ++i)
    {
        seq_t ai;
        if (i == 1)
            ai = 1;
        else if (i == 2)
            ai = 3;
        else if (i == 3)
            ai = 2;
        else
        {
            /* Determine p such that 2^p < i-1 <= 2^(p+1)  */
            idx_t p = floor_log2_idx(i - 2);

            /* 2^(p+1) */
            idx_t power = (idx_t)1 << (p + 1);

            /* m = 2*i - 3 - 2^(p+1)  */
            idx_t m = (idx_t)2 * i - 3 - power;

            seq_t a_m = values[m - 1];          /* a(m), 1‑based index */
            ai = prefix[i - 1] + a_m;           /* recurrence */
        }

        values[i - 1] = ai;
        prefix[i] = prefix[i - 1] + ai;
    }

    max_n = n;
    return values[n - 1];
}
