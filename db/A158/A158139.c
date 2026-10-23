/*
 * A158139  Number of nondecreasing integer sequences of length 5 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^5 P_exact(n,k) * P_atmost(n,5-k) where
 *            P_exact(n,k) = number of partitions of n into exactly k positive
 *            parts, and P_atmost(n,m) = number of partitions of n into at
 *            most m parts (non‑negative)
 * Range:     u8 n <= 12, u16 n <= 86, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* for n up to 10000 we keep a table of partitions into 0..5 parts.
   Each row contains 6 entries: columns 0..5.                     */
static seq_t *part_exact = NULL;   /* flattened array */
static idx_t  max_n_computed = 0;  /* highest n that has been filled */

/* ensure that part_exact[0..n][0..5] are computed */
static void ensure_partitions(idx_t n)
{
    if (n <= max_n_computed)
        return;                      /* already computed */

    /* reallocate to accommodate rows up to n */
    size_t new_rows = (size_t)(n + 1);
    size_t new_size = new_rows * (size_t)6;
    seq_t *new_ptr = NULL;

    if (part_exact == NULL) {
        /* first allocation */
        new_ptr = (seq_t *)malloc(new_size * sizeof(seq_t));
        for (size_t i = 0; i < new_size; ++i)
            new_ptr[i] = (seq_t)0;
        /* base case: partition of 0 into 0 parts */
        new_ptr[0 * 6 + 0] = (seq_t)1;
    } else {
        /* grow existing table */
        new_ptr = (seq_t *)realloc(part_exact, new_size * sizeof(seq_t));
    }

    /* compute rows from (max_n_computed+1) up to n */
    idx_t start = max_n_computed + 1;
    for (idx_t i = start; i <= n; ++i) {
        size_t base = (size_t)i * 6;
        new_ptr[base + 0] = (seq_t)0;          /* no partition of positive n into 0 parts */

        for (idx_t k = 1; k <= 5; ++k) {
            seq_t val = new_ptr[(size_t)(i - 1) * 6 + (k - 1)];
            if (i >= k)
                val += new_ptr[(size_t)(i - k) * 6 + k];
            new_ptr[base + k] = val;
        }
    }

    part_exact = new_ptr;
    max_n_computed = n;
}

/* Sequence A158139 */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined for n < 1 */

    ensure_partitions(n);

    seq_t result = (seq_t)0;
    for (idx_t k = 1; k <= 5; ++k) {
        seq_t exact = part_exact[(size_t)n * 6 + k];
        if (exact == 0)
            continue;

        idx_t m = 5 - k;
        seq_t atmost = (seq_t)0;
        for (idx_t t = 0; t <= m; ++t)
            atmost += part_exact[(size_t)n * 6 + t];

        result += exact * atmost;
    }
    return result;
}
