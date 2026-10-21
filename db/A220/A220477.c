/*
 * A220477  Total number of parts in all partitions of n with at least one
 *          distinct part.
 * https://oeis.org/A220477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A006128(n) – σ(n) * where A006128(n) is the total number
 *            of parts in all partitions of n * and σ(n) is the sum of the
 *            divisors of n. The implementation * recomputes the needed tables
 *            up to the requested n every time a * larger index is asked for.
 *            The values fit into the provided * seq_t ty...
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 100, u64 n <= 354,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_seq_array.h"
/* The following helper will free an array allocated with alloc_seq_array. */
#include "../utils/free_seq_array.h"

extern seq_t a(idx_t n);

/* --------------------------------------------------------------------- */
/* static storage for the tables */
static seq_t *part_counts = NULL;   /* p[n]  – number of partitions of n   */
static seq_t *total_parts = NULL;   /* t[n]  – total number of parts in all partitions of n   */
static seq_t *sigma_divs  = NULL;   /* s[n]  – sum of divisors of n        */
static idx_t  max_computed  = 0;    /* largest n for which the tables are up to date */

/* --------------------------------------------------------------------- */
/* Utility to (re)allocate all three arrays up to new_max (inclusive) */
static void
rebuild_tables(idx_t new_max)
{
    /* allocate temporary arrays */
    seq_t *tmp_p   = alloc_seq_array(new_max + 1);
    seq_t *tmp_t   = alloc_seq_array(new_max + 1);
    seq_t *tmp_s   = alloc_seq_array(new_max + 1);

    /* initialise to zero */
    for (idx_t i = 0; i <= new_max; ++i)
    {
        tmp_p[i] = 0;
        tmp_t[i] = 0;
        tmp_s[i] = 0;
    }

    /* base case for partitions */
    tmp_p[0] = 1;

    /* partition DP and total parts DP */
    for (idx_t part = 1; part <= new_max; ++part)
    {
        for (idx_t j = part; j <= new_max; ++j)
        {
            tmp_p[j]   += tmp_p[j - part];
            tmp_t[j]   += tmp_t[j - part] + tmp_p[j - part];
        }
    }

    /* sum of divisors (σ) */
    for (idx_t d = 1; d <= new_max; ++d)
    {
        for (idx_t j = d; j <= new_max; j += d)
            tmp_s[j] += d;
    }

    /* free old arrays if they exist */
    if (part_counts)  free_seq_array(part_counts);
    if (total_parts)  free_seq_array(total_parts);
    if (sigma_divs)   free_seq_array(sigma_divs);

    /* swap in the new arrays */
    part_counts = tmp_p;
    total_parts = tmp_t;
    sigma_divs  = tmp_s;
    max_computed = new_max;
}

/* --------------------------------------------------------------------- */
/* Ensure that all tables contain data up to n */
static void
ensure_computed(idx_t n)
{
    if (n <= max_computed)
        return;
    rebuild_tables(n);
}

/* --------------------------------------------------------------------- */
/* public function required by the harness */
seq_t
a(idx_t n)
{
    if (n == 0)          /* a(0) is outside the sequence; return 0 */
        return 0;

    ensure_computed(n);
    /* a(n) = A006128(n) – σ(n) */
    return total_parts[n] - sigma_divs[n];
}
