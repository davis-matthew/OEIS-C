/*
 * A109706  Number of partitions of n into parts each equal to 4 mod 7.
 * https://oeis.org/A109706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of partitions of n into parts each congruent to 4 (mod
 *            7)
 * Range:     u8 n <= 138, u16 n <= 332, u32 n <= 930, u64 n <= 2950,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 107 terms through n = 9291 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It is computed by ordinary dynamic programming with allowed parts 4,11,18,25,…  */
/* a(0) = 1; for n>0, a(n) = Σ_{p≡4 (mod 7)} a(n-p) where the sum is over all p ≤ n. */

/* The routine keeps a static table that can be extended on demand. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Allocate an array of seq_t, compatible with C and C++ */
static inline seq_t *alloc_seq_array(size_t cap)
{
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t *)malloc(cap * sizeof(seq_t));
#endif
}

/* Free an array of seq_t, compatible with C and C++ */
static inline void free_seq_array(seq_t *ptr)
{
#ifdef __cplusplus
    delete [] ptr;
#else
    free(ptr);
#endif
}

static seq_t *dp   = NULL;   /* dp[i] = a(i) for 0 ≤ i < dpsz */
static idx_t  dpsz = 0;      /* current table size */

/* Ensure that dp has entries for all indices up to n (inclusive). */
static void compute_upto(idx_t n)
{
    /* Return if already computed. */
    if (n + 1 <= dpsz) return;

    /* Allocate new table and initialize to 0. */
    idx_t new_sz = n + 1;
    seq_t *new_dp = alloc_seq_array((size_t)new_sz);
    for (idx_t i = 0; i < new_sz; ++i) new_dp[i] = 0;
    new_dp[0] = 1;      /* a(0) = 1 */

    /* Dynamic programming over all allowed part sizes (4, 11, 18, …). */
    for (idx_t part = 4; part <= n; part += 7)
    {
        for (idx_t i = part; i <= n; ++i)
        {
            new_dp[i] += new_dp[i - part];
        }
    }

    /* Replace old table. */
    if (dp != NULL) free_seq_array(dp);
    dp   = new_dp;
    dpsz = new_sz;
}

/* Public interface expected by the test harness. */
seq_t a(idx_t n)
{
    compute_upto(n);
    return dp[n];
}
