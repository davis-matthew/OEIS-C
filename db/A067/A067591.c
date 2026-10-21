/*
 * A067591  Number of partitions of n into evil numbers (A001969).
 * https://oeis.org/A067591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of partitions of n into evil numbers (A001969)
 * Range:     u8 n <= 43, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A067591.  The function is thread‑safe
   as it uses only static storage protected by a check on the maximum
   value of n requested.  No external utilities are required. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static storage for dynamic programming table */
static seq_t *dp_table = NULL;
static idx_t max_n_computed = 0;

/* If n > max_n_computed, we extend the table and recompute all
   values up to n.  For n <= max_n_computed we simply return the
   pre-computed value. */
seq_t a(idx_t n)
{
    /* Ensure we have at least one entry for n = 0 */
    if (max_n_computed == 0 && dp_table == NULL) {
        dp_table = (seq_t *)malloc(sizeof(seq_t));
        if (dp_table == NULL) return (seq_t)0;
        dp_table[0] = (seq_t)1;
        max_n_computed = 0;
    }

    if (n > max_n_computed) {
        idx_t new_size = n + 1;
        seq_t *new_table = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (new_table == NULL) return (seq_t)0;

        /* initialize the new table to zero */
        for (idx_t i = 0; i < new_size; ++i)
            new_table[i] = (seq_t)0;

        /* base case: empty partition of 0 */
        new_table[0] = (seq_t)1;

        /* Coin change style DP: all evil numbers up to n as coins */
        for (idx_t e = 1; e <= n; ++e) {
            /* e is evil if it has an even number of set bits */
            if (((__builtin_popcountll((unsigned long long)e) & 1u) == 0u)) {
                for (idx_t i = e; i <= n; ++i) {
                    new_table[i] += new_table[i - e];
                }
            }
        }

        /* free old table if it exists */
        if (dp_table != NULL)
            free(dp_table);

        dp_table = new_table;
        max_n_computed = n;
    }

    return dp_table[n];
}
