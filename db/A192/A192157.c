/*
 * A192157  Monotonic ordering of nonnegative differences 3^i-9^j, for
 *          40>=i>=0, j>=0.
 * https://oeis.org/A192157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   cmp_values
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: the n-th smallest non‑negative value of 3^i – 9^j,
   with integer indices 0 ≤ i ≤ 40, j ≥ 0, and i ≥ 2j.  The sequence
   is finite; this implementation precomputes all values once and
   returns them on demand. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cmp_values.h"

/* Static tables for the powers of 3, all differences,
   and the unique sorted list (max 441 entries). */
static seq_t pow3[41];
static seq_t values[441];
static seq_t uniq[441];
static size_t uniq_len = 0;
static bool initialized = false;

/* One‑time initializer that precomputes powers of three, all
   required differences, sorts them, and removes duplicates. */
static void init(void)
{
    if (initialized) return;

    /* Powers of 3 up to 3^40. */
    pow3[0] = (seq_t)1;
    for (size_t i = 1; i <= 40; ++i)
        pow3[i] = pow3[i - 1] * (seq_t)3;

    /* Generate all 3^i – 9^j where i ≥ 2j. */
    size_t cnt = 0;
    for (size_t i = 0; i <= 40; ++i) {
        size_t maxj = i / 2;               /* i ≥ 2j  →  j ≤ i/2 */
        for (size_t j = 0; j <= maxj; ++j)
            values[cnt++] = pow3[i] - pow3[2 * j];
    }

    /* Sort and deduplicate. */
    qsort(values, cnt, sizeof(seq_t), cmp_values);
    uniq_len = 0;
    if (cnt > 0) {
        uniq[uniq_len++] = values[0];
        for (size_t k = 1; k < cnt; ++k)
            if (cmp_values(&values[k], &values[k - 1]) != 0)
                uniq[uniq_len++] = values[k];
    }

    initialized = true;
}

/* Return the n-th term (1‑based).  Returns 0 if n exceeds the number of
   precomputed values; this occurs only for n larger than the finite
   length of the sequence. */
seq_t a(idx_t n)
{
    init();
    if (n == 0) return (seq_t)0;                 /* out of bounds */
    /* Convert to zero‑based index safely (n≥1). */
    if ((size_t)n - 1 >= uniq_len) return (seq_t)0;
    return uniq[(size_t)n - 1];
}
