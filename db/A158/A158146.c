/*
 * A158146  Number of nondecreasing integer sequences of length 12 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{r=1}^{12} Σ_{t=1}^{12-r} P(n,r)·P(n,t) where P(n,k) is
 *            the number of partitions of n into exactly k positive parts. For
 *            n ≤ 6, this equals A000041(n)^2. The implementation builds the
 *            partition table up to the requested n and accumulates the result
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 71, u64 n <= 474, u128 n <= 474,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* safety for n < 1 */

    const size_t STRIDE = 13;           /* k runs from 0..12 */
    static seq_t *part = NULL;          /* 1‑D array: part[n*STRIDE + k] */
    static idx_t  cache_max_n = 0;       /* largest n computed */

    /* Ensure table is up to date */
    if (n > cache_max_n) {
        size_t new_max_n = (size_t)n;
        size_t new_size  = (new_max_n + 1) * STRIDE;
        size_t old_size  = (size_t)(cache_max_n + 1) * STRIDE;

        seq_t *new_table = NULL;
        if (part == NULL) {
            new_table = (seq_t *)calloc(new_size, sizeof(seq_t));
        } else {
            new_table = (seq_t *)realloc(part, new_size * sizeof(seq_t));
            if (new_table) {
                /* Zero the newly allocated part of the array */
                for (size_t i = old_size; i < new_size; ++i)
                    new_table[i] = 0;
            }
        }
        if (new_table == NULL) return 0;   /* allocation failure */

        /* Initialize base case row for n = 0 */
        if (cache_max_n == 0) {
            new_table[0 * STRIDE + 0] = 1; /* p(0,0)=1 */
        }

        /* Compute partition numbers for rows (cache_max_n+1)..n */
        for (size_t row = cache_max_n + 1; row <= new_max_n; ++row) {
            /* p(row,0) = 0 for row>0 (already zero) */
            size_t max_k = row < 12 ? row : 12;
            for (size_t k = 1; k <= max_k; ++k) {
                seq_t val = 0;
                /* term p(row-1, k-1) */
                if (row >= 1)
                    val += new_table[(row - 1) * STRIDE + (k - 1)];
                /* term p(row-k, k) if row >= k */
                if (row >= k)
                    val += new_table[(row - k) * STRIDE + k];
                new_table[row * STRIDE + k] = val;
            }
        }

        part = new_table;
        cache_max_n = new_max_n;
    }

    /* Accumulate the answer using the computed partition table */
    seq_t sum = 0;
    const idx_t N = n;
    for (idx_t r = 1; r <= 12 && r <= N; ++r) {
        seq_t pr = part[(size_t)N * STRIDE + (size_t)r];
        if (pr == 0) continue;
        for (idx_t t = 1; t <= 12 && t <= N; ++t) {
            if (r + t > 12) continue;
            seq_t pt = part[(size_t)N * STRIDE + (size_t)t];
            if (pt == 0) continue;
            sum += pr * pt;
        }
    }

    return sum;
}
