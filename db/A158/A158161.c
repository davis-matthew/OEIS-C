/*
 * A158161  Number of nondecreasing integer sequences of length 27 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{27} Σ_{j=1}^{27-i} p_i(n) * p_j(n) where p_k(m)
 *            is the number of partitions of m into exactly k parts
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 44, u64 n <= 162, u128 n <= 259,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* utilities for dynamic allocation of seq_t arrays */
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

static seq_t *parts = NULL;          /* parts[n*28 + k] = p_k(n) (k=0..27) */
static idx_t  max_computed = 0;     /* largest n for which all rows are known */

/* ------------------------------------------------------------------ */
/* Ensure that the table 'parts' contains values for all n ≤ target   */
/* ------------------------------------------------------------------ */
static void ensure_partitions(idx_t target)
{
    if (target <= max_computed)
        return;

    size_t new_size   = (size_t)(target + 1) * 28;
    size_t old_size   = (size_t)(max_computed + 1) * 28;
    seq_t *new_parts  = seq_alloc_array(new_size);
    if (!new_parts)
        exit(1);

    /* copy old data */
    for (size_t i = 0; i < old_size; ++i)
        new_parts[i] = parts ? parts[i] : 0;

    /* initialise new cells to zero */
    for (size_t i = old_size; i < new_size; ++i)
        new_parts[i] = 0;

    /* compute new rows incrementally */
    for (idx_t n = max_computed + 1; n <= target; ++n)
    {
        size_t base = (size_t)n * 28;
        for (int k = 1; k <= 27; ++k)
        {
            seq_t val;
            if (k == 1)
            {
                /* exactly one part: always 1 */
                val = 1;
            }
            else if ((idx_t)k > n)
            {
                /* cannot partition n into more than n parts */
                val = 0;
            }
            else
            {
                /* recurrence: p_k(n) = p_k(n-k) + p_{k-1}(n-1) */
                val = new_parts[(n - k) * 28 + k] +
                      new_parts[(n - 1) * 28 + (k - 1)];
            }
            new_parts[base + k] = val;
        }
    }

    /* free old array and replace */
    if (parts)
        seq_free_array(parts);
    parts = new_parts;
    max_computed = target;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1, return 0 for safety */

    ensure_partitions(n);

    seq_t sum = 0;
    for (int i = 1; i <= 27; ++i)
    {
        seq_t pi = parts[(size_t)n * 28 + i];
        if (pi == 0)
            continue;
        for (int j = 1; j <= 27 - i; ++j)
        {
            seq_t pj = parts[(size_t)n * 28 + j];
            sum += pi * pj;
        }
    }
    return sum;
}
