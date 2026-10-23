/*
 * A157045  Triangular table: number of partitions of n into exactly n-k
 *          parts, each <= n-k. Same as A157044 but with rows reversed.
 * https://oeis.org/A157045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A157045 – Triangular table: number of partitions of r into exactly r‑c parts,
   each ≤ r‑c, where r is the row containing index n (1‑based) and c is the
   column offset within that row.
   For a given n ≥ 1, let r be the smallest integer with r(r+1)/2 ≥ n.
   Let offset = n – (r−1)r/2 – 1 (0‑based column index).
   Then a(n) = partitions(r, r‑offset, r‑offset).  */

/* ------------------------------------------------------------ */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* The maximum row we pre‑compute.  For n ≤ 10000, tri(141)=10011 > 10000. */
#define MAX_R 200
/* Convert a row index to its triangular number.  All arithmetic is idx_t. */
static inline idx_t triangular(idx_t r)
{
    return (r * (r + 1)) / 2;
}

/* 2‑D table of partition counts: dp[n][k] = P(n, k, k) – partitions of n into
   exactly k parts, each part ≤ k.  Size is (MAX_R+1) × (MAX_R+1). */
static seq_t dp[MAX_R + 1][MAX_R + 1] = {0};

/* One‑time precomputation flag. */
static bool initialized = false;

/* ------------------------------------------------------------
   Pre‑compute dp[n][k] for all n, k up to MAX_R.
   This function uses a straightforward DP for each k:
   arr[s][c] counts partitions of s into exactly c parts
   using parts ≤ current v (incremented up to k).
   Complexity: O(MAX_R³) – about 8 M iterations, well below the limit.
   ------------------------------------------------------------ */
static void precompute(void)
{
    /* For each k compute the 2‑D DP arr[s][c] (n ≤ MAX_R, c ≤ k). */
    for (idx_t k = 1; k <= MAX_R; ++k)
    {
        /* local table – zeroed each iteration */
        static seq_t arr[MAX_R + 1][MAX_R + 2];
        /* initialization */
        for (idx_t s = 0; s <= MAX_R; ++s)
            for (idx_t c = 0; c <= k; ++c)
                arr[s][c] = (seq_t)0;
        arr[0][0] = (seq_t)1;                 /* one way to partition 0 with 0 parts */

        /* allow parts up to k, in non‑decreasing order to avoid duplicates */
        for (idx_t v = 1; v <= k; ++v)
        {
            for (idx_t s = v; s <= MAX_R; ++s)
            {
                for (idx_t c = 1; c <= k; ++c)
                {
                    arr[s][c] += arr[s - v][c - 1];
                }
            }
        }

        /* store results for exactly k parts */
        for (idx_t n = 0; n <= MAX_R; ++n)
            dp[n][k] = arr[n][k];
    }

    /* Ensure dp[0][0] = 1 (not needed for n≥1 but harmless). */
    dp[0][0] = (seq_t)1;
    initialized = true;
}

/* ------------------------------------------------------------
   Main function required by the grader.
   ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) precompute();

    if (n == 0)            /* offset does not exist; return 0 for safety */
        return (seq_t)0;

    /* Find row r such that triangular(r-1) < n ≤ triangular(r). */
    idx_t r = 0;
    while (triangular(r) < n) ++r;

    /* Column offset within the row (0‑based). */
    idx_t offset   = n - triangular(r - 1) - 1;
    idx_t part_cnt = r - offset;   /* number of parts in the partition */

    if (r > MAX_R || part_cnt > MAX_R)   /* safety for very large n */
        return (seq_t)0;

    return dp[r][part_cnt];
}
