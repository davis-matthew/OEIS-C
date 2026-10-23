/*
 * A022555  Positive integers that are not the sum of two nonnegative cubes.
 * https://oeis.org/A022555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A022555: positive integers that are not the sum of two nonnegative cubes */
/* Computation: iterate through integers, keeping those that cannot be 
   expressed as i^3 + j^3 (i,j ≥ 0).  The sequence values are stored
   lazily in a static dynamic array.  */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_cube_root.h"

/* Test whether m can be written as i^3 + j^3 with i,j ≥ 0. */
static bool is_sum(seq_t m)
{
    idx_t max_i = (idx_t)floor_cube_root(m);
    for (idx_t i = 0; i <= max_i; ++i)
    {
        seq_t i3 = (seq_t)i * i * i;
        seq_t rem = m - i3;
        seq_t j = (seq_t)floor_cube_root(rem);
        seq_t j3 = j * j * j;
        if (j3 == rem)
            return true;
    }
    return false;
}

/* Static storage for the generated terms. */
static seq_t *seq = NULL;           /* array of sequence values          */
static size_t seq_len = 0;          /* number of values stored           */
static size_t seq_cap = 0;          /* allocated capacity                 */
static idx_t computed_max = 0;      /* largest integer examined so far   */

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0)            /* a(0) is undefined; return 0 for safety */
        return (seq_t)0;

    /* Generate terms until we have at least n stored values. */
    while (seq_len < (size_t)n)
    {
        /* Find the next integer that is NOT a sum of two cubes. */
        while (true)
        {
            ++computed_max;                     /* candidate integer   */
            if (!is_sum(computed_max))          /* not representable   */
            {
                /* Ensure capacity for the new term. */
                if (seq_len == seq_cap)
                {
                    seq_cap = seq_cap ? seq_cap * 2 : 64;
                    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                }
                seq[seq_len++] = (seq_t)computed_max;
                break;                          /* move on to next n   */
            }
        }
    }

    return seq[n - 1];
}
