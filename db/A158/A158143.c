/*
 * A158143  Number of nondecreasing integer sequences of length 9 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..9} sum_{ℓ=0..9-k} p(n,k) * p(n,ℓ) * where
 *            p(n,k) = number of partitions of n into exactly k positive
 *            parts. * This implementation precomputes p(n,k) for k ≤ 9 up to
 *            the maximal * queried n and then evaluates the double sum. The
 *            function is * memoised across calls, works for any o...
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 124, u64 n <= 643,
 *            u128 n <= 643, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define PARTS_MAX 10

static seq_t *partitions = NULL;   /*  (n+1) * PARTS_MAX elements  */
static idx_t  partitions_n = 0;    /* largest n for which we have data */

/* Ensure that partitions[0..n][0..9] are available */
static void
ensure_partitions(idx_t n)
{
    if (n <= partitions_n)
        return;

    idx_t new_n   = n;
    size_t new_sz = (new_n + 1) * PARTS_MAX;

    seq_t *new_part = (seq_t *)calloc(new_sz, sizeof(seq_t));
    if (!new_part) abort();               /* allocation failed */

    /* copy old data */
    if (partitions) {
        size_t old_sz = (partitions_n + 1) * PARTS_MAX;
        for (size_t i = 0; i < old_sz; ++i)
            new_part[i] = partitions[i];
        free(partitions);
    }

    /* initialise base case: p(0,0)=1 */
    if (new_part)
        new_part[0] = (seq_t)1;

    /* dynamic programming using the recurrence
       p(n,k) = p(n-1,k-1) + p(n-k,k)  for n>0, k>0; otherwise 0   */
    for (idx_t m = 1; m <= new_n; ++m) {
        for (size_t k = 0; k < PARTS_MAX; ++k) {
            seq_t val = 0;
            if (k > 0 && m >= 1) {
                val += new_part[(m - 1) * PARTS_MAX + (k - 1)];
            }
            if (k > 0 && m >= k) {
                val += new_part[(m - k) * PARTS_MAX + k];
            }
            new_part[m * PARTS_MAX + k] = val;
        }
    }

    partitions     = new_part;
    partitions_n   = new_n;
}

seq_t
a(idx_t n)
{
    if (n == 0) return 0;           /* out of offset, safe */
    ensure_partitions(n);

    /* cumulative sums S[ℓ] = sum_{i=0..ℓ} p(n,i)  */
    seq_t S[PARTS_MAX] = {0};
    S[0] = partitions[n * PARTS_MAX + 0];
    for (size_t i = 1; i < PARTS_MAX; ++i) {
        S[i] = S[i - 1] + partitions[n * PARTS_MAX + i];
    }

    seq_t result = 0;
    for (size_t k = 0; k < PARTS_MAX; ++k) {
        seq_t pk = partitions[n * PARTS_MAX + k];
        if (pk == 0) continue;
        size_t r = 9 - k;
        if (r > 9) r = 9;
        result += pk * S[r];
    }
    return result;
}
