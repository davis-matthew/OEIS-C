/*
 * A226622  Expansion of phi(x^2) / f(-x) in powers of x where phi(), f() are
 *          Ramanujan theta functions.
 * https://oeis.org/A226622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p(n) + 2 * Σ_{k≥1, 2k²≤n} p(n‑2k²) where p(m) is the
 *            partition number (A000041). This equals the expansion of
 *            (∑_{k∈ℤ} x^{2k²}) / Π_{k>0}(1‑x^k)
 * Range:     u8 n <= 13, u16 n <= 37, u32 n <= 115, u64 n <= 390,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The build supplies these types: seq_t, idx_t */
static seq_t *partitions = NULL;   /* cache of p[0..N] */
static idx_t  part_size  = 0;      /* current size of cache (entries 0..part_size‑1) */

/* Compute partition numbers up to at least 'n' using the generalized
   pentagonal number theorem.  The cache grows only when required. */
static void ensure_partitions(idx_t n)
{
    if (n + 1 <= part_size) return;          /* already available */

    idx_t new_size = n + 1;
    seq_t *new_partitions = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_partitions) return;             /* memory error: leave cache unchanged */

    /* copy old values */
    for (idx_t i = 0; i < part_size; ++i) {
        new_partitions[i] = partitions[i];
    }

    /* first partition number */
    new_partitions[0] = 1;

    /* compute new partition entries */
    idx_t start = part_size > 0 ? part_size : 1;  /* skip m==0 if already computed */
    for (idx_t m = start; m < new_size; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > m) break;
            if (k & 1) sum += new_partitions[m - pent1];
            else       sum -= new_partitions[m - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > m) continue;
            if (k & 1) sum += new_partitions[m - pent2];
            else       sum -= new_partitions[m - pent2];
        }
        new_partitions[m] = sum;
    }

    /* replace old cache */
    free(partitions);
    partitions = new_partitions;
    part_size  = new_size;
}

seq_t a(idx_t n)
{
    /* ensure partition numbers up to n are computed */
    ensure_partitions(n);

    seq_t result = partitions[n];
    for (idx_t k = 1; ; ++k) {
        idx_t two_k2 = 2 * k * k;
        if (two_k2 > n) break;
        result += (seq_t)2 * partitions[n - two_k2];
    }
    return result;
}
