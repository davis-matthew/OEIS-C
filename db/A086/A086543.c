/*
 * A086543  Number of partitions of n with at least one odd part.
 * https://oeis.org/A086543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) if n is odd a(n) = p(n) – p(n/2) if n is even where
 *            p(k) is the ordinary partition function (p(0)=1)
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 127, u64 n <= 416,
 *            u128 n <= 1458, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *part = NULL;   /* cache of p(0) … p(max_n) */
static idx_t  part_cap = 0;  /* actual size of the cache (max_n+1) */

/* Re-compute all partition numbers up to n using the simple
   “unbounded knapsack” DP:  
   p[0] = 1
   for coin = 1 .. n
       for i = coin .. n
           p[i] += p[i - coin]          */
static void recompute_partitions(idx_t n)
{
    size_t cap = (size_t)n + 1;
    seq_t *new_part = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!new_part)
        return;   /* out‑of‑memory – keep the old cache, if any */

    new_part[0] = (seq_t)1;
    for (size_t i = 1; i < cap; ++i)
        new_part[i] = (seq_t)0;

    for (size_t coin = 1; coin <= (size_t)n; ++coin)
        for (size_t i = coin; i <= (size_t)n; ++i)
            new_part[i] += new_part[i - coin];

    if (part)
        free(part);
    part     = new_part;
    part_cap = cap;
}

/* Ensure that part[0..n] is available.  If n is larger than the
   current cache size, recompute the whole table up to n.  */
static void ensure_partitions(idx_t n)
{
    if (part_cap > n)
        return;          /* already have data up to n */
    recompute_partitions(n);
}

/* OEIS A086543: number of partitions of n with at least one odd part. */
seq_t a(idx_t n)
{
    ensure_partitions(n);           /* obtain p(0)…p(n) */

    if ((n & 1) == 0)   /* n is even */
        return part[n] - part[n / 2];
    else
        return part[n];   /* n is odd */
}
