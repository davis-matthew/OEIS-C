/*
 * A034999  Number of ways to cut a 2 X n rectangle into rectangles with
 *          integer sides.
 * https://oeis.org/A034999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 2, a(2) = 8, and for n ≥ 3: a(n) = 6*a(n‑1) –
 *            7*a(n‑2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *vals   = NULL;   /* array of a(0)..a(max_computed) */
    static idx_t  size   = 0;      /* current allocated size (≥ max_computed+1) */
    static idx_t  max_computed = 2;/* last index that has a value */

    /* First call: allocate space for the three known terms and set them. */
    if (size == 0) {
        size = 3;
        vals = (seq_t *)malloc(size * sizeof(seq_t));
        vals[0] = (seq_t)1;
        vals[1] = (seq_t)2;
        vals[2] = (seq_t)8;
    }

    /* Return if already have the value. */
    if (n <= max_computed)
        return vals[n];

    /* Ensure capacity large enough to store up to n. */
    if (size <= n) {
        idx_t new_size = size;
        /* Grow exponentially to keep amortised linear cost. */
        do { new_size <<= 1; } while (new_size <= n);
        seq_t *tmp = (seq_t *)realloc(vals, new_size * sizeof(seq_t));
        if (!tmp) /* Out of memory; caller may handle. */
            return 0;
        vals = tmp;
        size = new_size;
    }

    /* Compute missing values up to n using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        vals[i] = (seq_t)6 * vals[i-1] - (seq_t)7 * vals[i-2];
    }
    max_computed = n;
    return vals[n];
}
