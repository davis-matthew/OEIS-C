/*
 * A171646  a(1) = 1, then partial products of Product_{n>=1}
 *          (p(n)/p(n-1)*p(n)/p(n-1)) =
 *          1*1*1*(2)*(2)*(3/2)*(3/2)*(5/3)*(5/3)*(7/5)*(7/5)*...*; p =
 *          partition numbers, A000041 starting (1, 2, 3, 5, ...).
 * https://oeis.org/A171646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 33, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A171646
 * a(1) = 1, then partial products of
 *   (p(1)/p(0))·(p(1)/p(0))·(p(2)/p(1))·(p(2)/p(1))·(p(3)/p(2))·(p(3)/p(2))·...
 * where p(k) are the partition numbers A000041.
 *
 * Formally, let k = ⌊n/2⌋. Then
 *   a(n) = p(k)²              if n is odd
 *   a(n) = p(k) · p(k-1)      if n is even
 * (with p(0) = 1).
 * The function below memoizes the partition numbers up to the largest
 * required k.  It recomputes the full table when a larger k is needed.
 * This satisfies the required performance for n up to ~10000.
 */

#include <stdlib.h>

/* Compute the partition numbers p(0..m) and store them in parts[0..m].
 * The array parts must already be allocated with at least m+1 entries. */
static void compute_partitions(idx_t m, seq_t *parts)
{
    /* Zero the table except for p(0)=1. */
    parts[0] = (seq_t)1;
    if (m > 0) {
        for (idx_t i = 1; i <= m; ++i) parts[i] = (seq_t)0;
    }

    /* Classic coin‑change DP: for each coin value c from 1..m,
     * add its contribution to all sums s >= c. */
    for (idx_t coin = 1; coin <= m; ++coin) {
        for (idx_t s = coin; s <= m; ++s) {
            parts[s] += parts[s - coin];
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* not used, but harmless */

    idx_t k = n / 2; /* floor(n/2) */

    /* Static cache of partition numbers.  Allocate once and grow when needed. */
    static idx_t part_limit = 0;
    static seq_t *parts      = NULL;

    if (parts == NULL) {
        /* First time: allocate and compute up to k. */
        parts = (seq_t *)malloc((size_t)(k + 1) * sizeof(seq_t));
        if (!parts) return (seq_t)0;          /* memory failure, safe error value */
        compute_partitions(k, parts);
        part_limit = k;
    }
    else if (k > part_limit) {
        /* Need larger table: reallocate and recompute from scratch. */
        seq_t *tmp = (seq_t *)realloc(parts, (size_t)(k + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* memory failure */
        parts = tmp;
        compute_partitions(k, parts);
        part_limit = k;
    }

    /* Apply the formula. */
    if (n & 1) { /* odd */
        return parts[k] * parts[k];
    } else {     /* even */
        if (k == 0) return (seq_t)1; /* n=0 case, not reached */
        return parts[k] * parts[k - 1];
    }
}
