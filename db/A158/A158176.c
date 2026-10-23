/*
 * A158176  Number of nondecreasing integer sequences of length 42 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 43, u64 n <= 134, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A158176: Number of nondecreasing integer sequences of length 42
   with sum zero and sum of absolute values 2n.
   a(n) = Σ_{p=1}^{min(n,42)} Σ_{q=1}^{min(n,42-p)} Pp(n) * Pq(n)
   where Pk(n) is the number of partitions of n into exactly k parts. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static seq_t *partitions = NULL;   /* partitions[n*43 + k] = number of partitions of n into exactly k parts (0 <= k <= 42) */
static idx_t  max_n     = 0;

/* Ensure that partition numbers up to n are computed. */
static void ensure_partitions(idx_t n)
{
    if (n <= max_n) return;

    size_t new_rows = (size_t)n + 1;
    size_t old_rows = (size_t)max_n + 1;

    if (partitions == NULL) {
        partitions = (seq_t *)calloc(new_rows * 43, sizeof(seq_t));
        if (!partitions) abort();
    } else {
        partitions = (seq_t *)realloc(partitions, new_rows * 43 * sizeof(seq_t));
        if (!partitions) abort();
        /* zero the newly added rows */
        memset(partitions + old_rows * 43, 0,
               (new_rows - old_rows) * 43 * sizeof(seq_t));
    }

    /* Base case: partition of 0 into 0 parts */
    if (max_n == 0) {
        partitions[0 * 43 + 0] = 1;
    }

    /* Compute rows from max_n+1 to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        for (int k = 1; k <= 42 && (idx_t)k <= i; ++k) {
            seq_t val = partitions[(i - 1) * 43 + (k - 1)];
            val += partitions[(i - k) * 43 + k];
            partitions[i * 43 + k] = val;
        }
    }

    max_n = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is defined for n >= 1 */

    ensure_partitions(n);

    seq_t result = 0;
    idx_t p_max = n < 42 ? n : 42;

    for (idx_t p = 1; p <= p_max; ++p) {
        seq_t Pp = partitions[n * 43 + p];
        if (Pp == 0) continue;

        idx_t q_limit = 42 - p;
        if (q_limit > n) q_limit = n;
        if (q_limit == 0) continue;

        for (idx_t q = 1; q <= q_limit; ++q) {
            seq_t Pq = partitions[n * 43 + q];
            if (Pq == 0) continue;
            result += Pp * Pq;
        }
    }

    return result;
}
