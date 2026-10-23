/*
 * A048163  a(n) = Sum_{k=1..n} ((k-1)!)^2*Stirling2(n,k)^2.
 * https://oeis.org/A048163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A048163: a(n) = sum_{k=1..n} ((k-1)!)^2 * Stirling2(n,k)^2
   (offset 1).  The implementation keeps a cache of Stirling row
   S(n,k), factorials up to n, and the already‑computed a(n).  Sequences
   are stored in `seq_t`.                                             */
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static idx_t table_size = 0;          /* number of entries allocated (>= n+1) */
static idx_t computed_n = 0;          /* largest n for which a(n) is ready */

/* tables: 0 .. table_size-1  (index 0 unused for a(n) and Stirling)  */
static seq_t *S_prev = NULL;          /* row n-1  of Stirling numbers   */
static seq_t *S_curr = NULL;          /* row n    of Stirling numbers   */
static seq_t *fact   = NULL;          /* fact[i] = i!                    */
static seq_t *a_vals = NULL;          /* a_vals[n] = a(n)                */

/* Allocate / enlarge the tables so that they hold indices 0..n.
   Existing data up to `table_size-1` is preserved.                */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= table_size) return;          /* already large enough */

    idx_t new_cap = n + 1;                     /* new size (index 0 .. n) */

    /* allocate new arrays (alloc_seq_array constructs the objects) */
    seq_t *new_S_prev = alloc_seq_array(new_cap);
    seq_t *new_S_curr = alloc_seq_array(new_cap);
    seq_t *new_fact   = alloc_seq_array(new_cap);
    seq_t *new_a_vals = alloc_seq_array(new_cap);

    /* Initialize all new slots to zero */
    for (idx_t i = 0; i < new_cap; ++i) {
        new_S_prev[i] = 0;
        new_S_curr[i] = 0;
        new_fact[i]   = 0;
        new_a_vals[i] = 0;
    }

    /* Copy previously computed data into the new arrays */
    if (table_size > 0) {
        for (idx_t i = 0; i < table_size; ++i) {
            new_S_prev[i] = S_prev[i];
            new_S_curr[i] = S_curr[i];
            new_fact[i]   = fact[i];
            new_a_vals[i] = a_vals[i];
        }
    }

    /* Compute missing factorials */
    for (idx_t i = table_size; i <= n; ++i) {
        if (i == 0)
            new_fact[i] = 1;
        else
            new_fact[i] = new_fact[i - 1] * (seq_t)i;
    }

    /* If this is the first allocation, set the base Stirling value
       S(0,0) = 1  (S_prev[0] will be used in the next computation). */
    if (table_size == 0) {
        new_S_prev[0] = 1;   /* S(0,0) = 1                */
        new_fact[0]   = 1;   /* 0! = 1                     */
    }

    /* Free old arrays */
    if (S_prev) free_seq_array(S_prev);
    if (S_curr) free_seq_array(S_curr);
    if (fact)   free_seq_array(fact);
    if (a_vals) free_seq_array(a_vals);

    /* Adopt the new arrays */
    S_prev  = new_S_prev;
    S_curr  = new_S_curr;
    fact    = new_fact;
    a_vals  = new_a_vals;
    table_size = new_cap;
}

/* Main function returning a(n).  Calls are in any order; already
   computed values are cached.                                            */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is defined for n>=1 */

    /* Make sure the internal tables are large enough for indices up to n */
    ensure_capacity(n);

    /* Compute any missing terms between computed_n + 1 and n */
    for (idx_t m = computed_n + 1; m <= n; ++m) {
        /* Stirling row S(m,k): start with S_curr[0] = 0 */
        S_curr[0] = 0;

        for (idx_t k = 1; k <= m; ++k) {
            /* S(m,k) = k * S(m-1,k) + S(m-1,k-1) */
            seq_t term1 = (seq_t)k * S_prev[k];
            seq_t term2 = S_prev[k - 1];
            S_curr[k] = term1 + term2;
        }

        /* Accumulate the sum for a(m) */
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k) {
            seq_t s2 = S_curr[k];
            seq_t s2_sq = s2 * s2;
            seq_t f = fact[k - 1];           /* (k-1)!                */
            seq_t f_sq = f * f;              /* ((k-1)!)^2            */
            sum += s2_sq * f_sq;
        }
        a_vals[m] = sum;

        /* Prepare for next iteration: current row becomes previous */
        seq_t *tmp = S_prev;
        S_prev = S_curr;
        S_curr = tmp;
        /* Next S_curr will be overwritten */
    }

    computed_n = n;   /* update the maximum computed index */

    return a_vals[n];
}
