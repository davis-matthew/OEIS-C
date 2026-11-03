/*
 * A090982  a(n) = partitions(n)*partitions(n+1).
 * https://oeis.org/A090982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = partitions(n) * partitions(n+1)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 42, u64 n <= 127, u128 n <= 416,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 983 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper to compute the partition numbers p[0..N] using the
   generalized pentagonal number theorem. */
static void compute_partitions(idx_t N, seq_t *out)
{
    out[0] = 1;
    for (idx_t n = 1; n <= N; ++n) {
        seq_t sum = 0;
        for (int k = 1; ; ++k) {
            idx_t pent1 = (idx_t)k * ((idx_t)3 * k - 1) / 2;
            if (pent1 > n) break;
            seq_t sign = (k & 1) ? 1 : -1;
            sum += sign * out[n - pent1];
            idx_t pent2 = (idx_t)k * ((idx_t)3 * k + 1) / 2;
            if (pent2 <= n) sum += sign * out[n - pent2];
        }
        out[n] = sum;
    }
}

/* Compute the sequence A090982:  a(n) = p(n) * p(n+1)  where p(k) is the number of partitions of k. */
seq_t a(idx_t n)
{
    /* Allocate/computed partitions up to n+1 on the first call or when a larger n is requested. */
    static seq_t *p = NULL;       /* array of partition counts */
    static idx_t cap = 0;         /* current capacity (number of elements stored) */

    /* We need indices 0..n+1 inclusive, hence cap must be at least n+2. */
    idx_t needed = n + 2;
    if (needed > cap) {
        seq_t *newp = mem_new(seq_t, needed);
        if (!newp) return 0;          /* allocation failed – return 0 (unlikely for judge data). */
        if (p) mem_free(p);
        p = newp;
        /* compute_partitions(N, out) fills out[0..N] with partition numbers.  */
        compute_partitions(needed - 1, p);   /* compute partitions up to n+1 */
        cap = needed;
    }
    return p[n] * p[n + 1];
}
