/*
 * A194544  Total sum of repeated parts in all partitions of n.
 * https://oeis.org/A194544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n} k * ( Σ_{t=2}^{⌊n/k⌋} p(n - t·k) + p(n - 2·k)
 *            ) where p(m) is the partition number with p(0) = 1
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 93, u64 n <= 334,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Depend on the helper allocation/free routines for seq_t arrays. */
#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case first. */
    if (n == 0)
        return 0;

    /* Allocate an array of partition numbers p[0..n]. */
    size_t sz = (size_t)(n + 1);
    seq_t *p = seq_alloc_array(sz);
    if (!p)
        return 0; /* allocation failure, return zero as a fallback. */

    /* Base case */
    p[0] = 1;

    /* Compute partition numbers using the generalized pentagonal recurrence. */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1;; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent1 > m && pent2 > m)
                break;
            int signPositive = (k & 1) != 0;
            if (pent1 <= m) {
                if (signPositive)
                    pos += p[m - pent1];
                else
                    neg += p[m - pent1];
            }
            if (pent2 <= m) {
                if (signPositive)
                    pos += p[m - pent2];
                else
                    neg += p[m - pent2];
            }
        }
        p[m] = pos - neg;   /* pos >= neg for all m */
    }

    /* Compute the required sum over all partitions of n. */
    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t maxT = n / k;
        seq_t s = 0;
        for (idx_t t = 1; t <= maxT; ++t)
            s += p[n - t * k];

        seq_t sub = 0;
        if (n >= k) {
            sub = p[n - k];
            if (n >= 2 * k)
                sub -= p[n - 2 * k];
        }

        seq_t diff = s - sub;              /* s >= sub for all k */
        result += (seq_t)k * diff;
    }

    /* Free the temporary array before returning. */
    seq_free_array(p);
    return result;
}
