/*
 * A058682  a(n) = p(0) + p(1) + ... + p(n) - n - 1, where p = partition
 *          numbers, A000041.
 * https://oeis.org/A058682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} p(k) – n – 1 where p(k) are the partition
 *            numbers of k (OEIS A000041)
 * Range:     u8 n <= 11, u16 n <= 33, u32 n <= 107, u64 n <= 372,
 *            u128 n <= 1357, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation recomputes the partitions up to n on each call.
   It uses the pentagonal‑number recurrence and stores only the
   values needed up to n.  All arithmetic is performed in the
   bigint type seq_t; the index type idx_t is an unsigned integer. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate space for partition numbers p[0..n] */
    seq_t *p = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!p) return (seq_t)0;            /* OOM fallback – never happens for n ≤ 10000 */

    p[0] = (seq_t)1;                    /* p(0) = 1 */
    seq_t sum = p[0];                   /* running sum Σ p(k) */

    for (idx_t m = 1; m <= n; ++m) {
        seq_t total = 0;
        for (idx_t k = 1; ; ++k) {
            /* Generalised pentagonal numbers */
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > m) break;

            /* Add or subtract p(m - pent1) according to parity of k */
            total += (k & 1) ? p[m - pent1] : -p[m - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > m) continue;
            total += (k & 1) ? p[m - pent2] : -p[m - pent2];
        }
        p[m] = total;
        sum += total;
    }

    seq_t result = sum - (seq_t)n - 1;
    free(p);
    return result;
}
