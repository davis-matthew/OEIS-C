/*
 * A157044  Triangle read by rows: T(n,k) = number of partitions of n into
 *          exactly k parts, each <= k.
 * https://oeis.org/A157044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of partitions of r into exactly k parts each <= k,
 *            where r and k are derived from the 1‑based index n by reading
 *            the triangle row by row
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include "../utils/seq_one.h"

#define MAX_N 200 /* enough for n up to ~20000 (row 200 gives 20100 indices) */

static seq_t memo[MAX_N][MAX_N][MAX_N];
static bool vis[MAX_N][MAX_N][MAX_N] = { false };

/* Recursive helper: partitions of sum 'n' into exactly 'p' parts,
   each part <= 'm' (parts are non‑increasing to avoid duplicates).  */
static seq_t f_rec(idx_t n, idx_t p, idx_t m)
{
    if (p == 0)
        return (n == 0) ? seq_one() : (seq_t)0;
    if (n == 0)
        return (seq_t)0;
    if (m == 0)
        return (seq_t)0;

    if (vis[n][p][m])
        return memo[n][p][m];

    seq_t res = 0;

    /* Do not use part of size m. */
    res += f_rec(n, p, m - 1);

    /* Use at least one part of size m. */
    if (m <= n)
        res += f_rec(n - m, p - 1, m);

    vis[n][p][m] = true;
    memo[n][p][m] = res;
    return res;
}

/* Number of partitions of 'n' into exactly 'k' parts each <= 'k'. */
static seq_t compute_T(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    return f_rec(n, k, k);
}

seq_t a(idx_t n)
{
    /* find row r and column k from linear index n (1‑based) */
    idx_t r = (idx_t)((sqrt(1.0 + 8.0 * (double)n) - 1.0) / 2.0);
    while (r * (r + 1) / 2 < n)
        ++r;
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;

    return compute_T(r, k);
}
