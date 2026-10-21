/*
 * A076411  Number of perfect powers < n.
 * https://oeis.org/A076411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of perfect powers < n (OEIS A076411). */
/* ...
   a(n) = |{ m : m < n and m = a^b for some integers a≥1, b≥2 }|   */

/* The implementation enumerates all a^b < n, stores the distinct values,
   and returns their count.  For n ≤ 1 the result is 0, otherwise 1 (for
   the perfect power 1) plus the number of distinct powers with base ≥ 2. */

#include <stdlib.h>
#include <stdint.h>

/* Integer floor square root for unsigned 64‑bit values.
   Returns the largest integer r such that r*r ≤ n.           */
static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > n / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* Comparator for idx_t values used by qsort.                */
static int cmp_idx_t(const void *a, const void *b)
{
    idx_t v1 = *(const idx_t *)a;
    idx_t v2 = *(const idx_t *)b;
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

/* Main OEIS sequence function.                               */
seq_t a(idx_t n)
{
    /* No perfect power < 1. */
    if (n <= 1)
        return 0;

    idx_t max_val   = n - 1;            /* largest number to consider    */
    idx_t base_limit = floor_sqrt_u64(max_val);

    /* Dynamic array collecting all perfect powers < n.      */
    idx_t *vec = NULL;
    size_t vec_cap = 0, vec_len = 0;

    for (idx_t base = 2; base <= base_limit; ++base) {
        idx_t power = base * base;      /* exponent 2 */
        while (power <= max_val) {
            /* Grow array if necessary.   */
            if (vec_len == vec_cap) {
                size_t new_cap = vec_cap ? vec_cap << 1 : 8;
                idx_t *tmp = (idx_t *)realloc(vec, new_cap * sizeof(idx_t));
                if (!tmp) {             /* out of memory                       */
                    free(vec);
                    return (seq_t)(-1); /* sentinel for fatal error            */
                }
                vec = tmp;
                vec_cap = new_cap;
            }
            vec[vec_len++] = power;

            /* Stop to avoid overflow in the next multiplication. */
            if (power > max_val / base)
                break;
            power *= base;
        }
    }

    /* Count unique values.                                */
    size_t uniq = 0;
    if (vec_len > 0) {
        qsort(vec, vec_len, sizeof(idx_t), cmp_idx_t);
        uniq = 1;
        for (size_t i = 1; i < vec_len; ++i)
            if (vec[i] != vec[i - 1])
                ++uniq;
    }

    free(vec);                       /* clean up temporary storage        */

    /* 1 is always a perfect power (1^k).        */
    return (seq_t)(uniq + 1u);
}
