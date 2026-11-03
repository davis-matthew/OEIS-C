/*
 * A076087  a(n) = 7*n - 3 * Sum_{i=1..n} A006460(i).
 * https://oeis.org/A076087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 7*n - 3*∑_{i=1..n} A006460(i) where A006460(i) is the
 *            eventual Collatz limit (1, 2 or 4) obtained by iterating the
 *            Collatz map in blocks of three steps until a fixed point {1,2,4}
 *            is reached
 * Range:     i8 n <= 14, i16 n <= 87, i32 n <= 87, i64 n <= 87, i128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t collatz_limit(seq_t x)
{
    while (1) {
        for (int step = 0; step < 3; ++step) {
            if ((x & 1) == 0)           /* even */
                x >>= 1;
            else                        /* odd */
                x = 3 * x + 1;
        }
        if (x == 1 || x == 2 || x == 4)
            return x;
    }
}

/* Prefix sums of the A006460(i) values.
   prefix[i-1] = Σ_{j=1..i} b(j). */
static seq_t *prefix = NULL;      /* array of cumulative sums */
static idx_t prefix_len = 0;      /* number of entries already computed */
static idx_t prefix_cap = 0;      /* currently allocated capacity */

static void ensure_prefix_capacity(idx_t needed)
{
    if (needed <= prefix_cap)
        return;
    idx_t new_cap = needed * 2;
    seq_t *new_ptr = mem_resize(seq_t, prefix, new_cap);
    prefix = new_ptr;
    prefix_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)                /* behaviour below the offset is undefined */
        return (seq_t)0;

    /* Fill prefix sums up to n if they are not already available. */
    if (n > prefix_len) {
        ensure_prefix_capacity(n);
        seq_t curr_sum = (prefix_len > 0) ? prefix[prefix_len - 1] : (seq_t)0;
        for (idx_t i = prefix_len + 1; i <= n; ++i) {
            seq_t val = collatz_limit((seq_t)i);
            curr_sum += val;
            prefix[i - 1] = curr_sum;
        }
        prefix_len = n;
    }

    /* Compute a(n) using the stored cumulative sum. */
    seq_t sum_b = prefix[n - 1];
    return (seq_t)7 * (seq_t)n - (seq_t)3 * sum_b;
}
