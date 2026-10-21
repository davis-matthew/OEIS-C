/*
 * A096533  Number of composite numbers not greater than the n-th composite
 *          number that divide at least one other number not greater than the
 *          n-th composite number.
 * https://oeis.org/A096533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = A065855(floor(composite_n/2))
 * where composite_n is the n‑th composite number.
 * We generate composites on demand, cache them, and
 * count how many composites are ≤ floor(composite_n/2)
 * using a binary search. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache of composites */
    static seq_t *composites = NULL;
    static size_t comp_cnt  = 0;
    static size_t comp_cap  = 0;
    static idx_t  next_candidate = 4;

    /* generate composites up to the n‑th one if needed */
    while (comp_cnt < (size_t)n)
    {
        /* grow the array if necessary */
        if (comp_cnt == comp_cap)
        {
            size_t new_cap = comp_cap ? (comp_cap << 1) : 64;
            seq_t *tmp = (seq_t *)realloc(composites, new_cap * sizeof(seq_t));
            composites = tmp;
            comp_cap = new_cap;
        }

        /* test current candidate */
        if (!is_prime(next_candidate))
            composites[comp_cnt++] = next_candidate;

        next_candidate++;
    }

    /* bound = floor(composite_n / 2) */
    seq_t bound = composites[(size_t)n - 1] >> 1;

    /* binary search: number of composites <= bound */
    size_t left = 0, right = comp_cnt;
    while (left < right)
    {
        size_t mid = left + ((right - left) >> 1);
        if (composites[mid] <= bound)
            left = mid + 1;
        else
            right = mid;
    }

    return (seq_t)left;
}
