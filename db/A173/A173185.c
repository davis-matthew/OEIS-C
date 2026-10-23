/*
 * A173185  Partial sums of A003418.
 * https://oeis.org/A173185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=0..n} lcm(1..i) (partial sums of A003418)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"   /* gcd(seq_t, seq_t) */

/* ------------------------------------------------------------------
 * Static cache of LCM values and their partial sums.
 * -------------------------------------------------------------- */
static seq_t *partial_sum = NULL;   /* a(n) */
static seq_t *lcm_up_to   = NULL;   /* lcm(1..n) for each n */
static idx_t  computed    = 0;      /* highest index already computed */
static size_t capacity    = 0;      /* size of allocated arrays */

/* ------------------------------------------------------------------
 * Grow the cached arrays so that indices 0..n are valid.
 * The new capacity is n+1.  Existing values are preserved.
 * -------------------------------------------------------------- */
static void
ensure_capacity(idx_t n)
{
    size_t new_cap = (size_t)n + 1;           /* need indices 0..n */

    if (new_cap <= capacity)
        return;                              /* already large enough */

    /* Allocate or grow the arrays */
    seq_t *new_partial = NULL;
    seq_t *new_lcm     = NULL;

    if (partial_sum == NULL || lcm_up_to == NULL) {
        /* First allocation */
        new_partial = (seq_t *)malloc(new_cap * sizeof(seq_t));
        new_lcm     = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        /* Grow existing arrays */
        new_partial = (seq_t *)realloc(partial_sum, new_cap * sizeof(seq_t));
        new_lcm     = (seq_t *)realloc(lcm_up_to,   new_cap * sizeof(seq_t));
    }

    /* On a realloc failure the program aborts – acceptable for OEIS
       evaluation environment. */
    partial_sum = new_partial;
    lcm_up_to   = new_lcm;
    capacity    = new_cap;

    /* Initialise any new entries to 0 (they will be overwritten) */
    for (size_t i = computed + 1; i < capacity; ++i) {
        partial_sum[i] = (seq_t)0;
        lcm_up_to[i]   = (seq_t)0;
    }
}

/* ------------------------------------------------------------------
 * Main function: compute a(n) as the partial sum of lcm(1..i).
 * -------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    /* Ensure we have the base values for n==0 */
    if (capacity == 0) {
        ensure_capacity(0);
        lcm_up_to[0]   = (seq_t)1;   /* lcm(1..0) = 1 by definition */
        partial_sum[0] = (seq_t)1;   /* a(0) = 1 */
        computed = 0;
    }

    if (n > (idx_t)(capacity - 1))
        ensure_capacity(n);

    /* If we need higher values, extend the cache */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t prev = lcm_up_to[i - 1];
        seq_t g    = gcd(prev, (seq_t)i);
        seq_t temp = prev / g;                      /* avoid overflow */
        seq_t next_lcm = temp * (seq_t)i;

        lcm_up_to[i]   = next_lcm;
        partial_sum[i] = partial_sum[i - 1] + next_lcm;
    }

    computed = n;
    return partial_sum[n];
}
