/*
 * A158167  Number of nondecreasing integer sequences of length 33 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k+l ≤ 33} p_k(n) · p_l(n) where p_k(n) is the number
 *            of partitions of n into exactly k parts. For n ≤ 16 this equals
 *            A000041(n)²; for larger n the restriction k+l≤33 reduces the
 *            count
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 43, u64 n <= 145, u128 n <= 230,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation precomputes p_k(n) using the recurrence
    p_k(n) = p_{k-1}(n-1) + p_k(n-k)  (n ≥ k ≥ 1),  with p_k(0)=0 for k>0,
    p_0(0)=1.  All values are kept in a flat array of seq_t.  The sequence
    a(n) is then computed as the sum of all pairwise products p_k(n)p_l(n)
    with k+l≤33. */

#include <stdlib.h>
#include <string.h>

#define KMAX 33                     /* maximum number of parts to consider */
#define KSIZE (KMAX + 1)            /* array dimension per row */

/* forward declaration of the required function */
seq_t a(idx_t n);

/* Flat 2‑D array: dp[n*KSIZE + k] holds p_k(n) */
static seq_t *dp = NULL;
static idx_t dp_max_n = 0;          /* largest n that has been built */

/* -------------------------------------------------------------------------- */
/*  Ensure dp contains values up to the requested index n.                     */
/* -------------------------------------------------------------------------- */
static void ensure_dp(idx_t n)
{
    if (n <= dp_max_n) return;      /* already built */

    /* allocate a new array of the required size, zero‑initialized      */
    size_t new_size = (size_t)(n + 1) * (size_t)KSIZE;
    seq_t *new_dp = (seq_t *)calloc(new_size, sizeof(seq_t));
    if (!new_dp) exit(1);

    /* copy existing data if any                                   */
    if (dp) {
        size_t old_size = (size_t)(dp_max_n + 1) * (size_t)KSIZE;
        memcpy(new_dp, dp, old_size * sizeof(seq_t));
        free(dp);
    }

    dp = new_dp;

    /* p_0(0) = 1, all other entries remain zero (calloc did that)     */
    dp[0 * KSIZE + 0] = 1;

    /* build values for rows dp_max_n + 1 … n                           */
    for (idx_t nn = dp_max_n + 1; nn <= n; ++nn) {
        for (idx_t k = 1; k <= KMAX; ++k) {
            seq_t val = 0;
            if (nn >= k) {
                seq_t part1 = dp[(nn - 1) * KSIZE + (k - 1)];
                seq_t part2 = dp[(nn - k) * KSIZE + k];
                val = part1 + part2;
            }
            dp[nn * KSIZE + k] = val;
        }
    }

    dp_max_n = n;
}

/* -------------------------------------------------------------------------- */
/*  Main entry point of the sequence: a(n).                                    */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_dp(n);

    seq_t *row = &dp[n * KSIZE];     /* array of p_k(n), k = 0..KMAX */
    seq_t result = 0;

    for (idx_t k = 0; k <= KMAX; ++k) {
        seq_t pk = row[k];
        if (pk == 0) continue;       /* skip zero entries */
        for (idx_t l = 0; l <= KMAX - k; ++l) {
            seq_t pl = row[l];
            if (pl == 0) continue;
            result += pk * pl;
        }
    }
    return result;
}
