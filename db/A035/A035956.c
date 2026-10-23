/*
 * A035956  Number of partitions of n into parts not of the form 15k, 15k+2 or
 *          15k-2. Also number of partitions with 1 part of size 1 and
 *          differences between parts at distance 6 are greater than 1.
 * https://oeis.org/A035956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the number of partitions of n into parts not congruent to
 *            0, 2 or 13 modulo 15
 * Range:     u8 n <= 20, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *partitions = NULL;
static idx_t max_cached_n = 0;

/* Recompute all values up to n (inclusive). */
static void recompute(idx_t n)
{
#if __cplusplus
    delete[] partitions;
    partitions = new seq_t[n + 1];
#else
    free(partitions);
    partitions = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
#endif
    partitions[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) partitions[i] = (seq_t)0;

    for (idx_t p = 1; p <= n; ++p) {
        idx_t r = p % 15;
        if (r == 0 || r == 2 || r == 13) continue;      // forbidden residues
        for (idx_t j = p; j <= n; ++j) {
            partitions[j] += partitions[j - p];
        }
    }
    max_cached_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below the OEIS offset – trivial */
    if (n > max_cached_n) recompute(n);
    return partitions[n];
}
