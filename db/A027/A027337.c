/*
 * A027337  Number of partitions of n that do not contain 3 as a part.
 * https://oeis.org/A027337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) - p(n-3) where p(n) is the partition function
 *            (p(0)=1)
 * Range:     u8 n <= 18, u16 n <= 48, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cache of p(i) for i = 0 .. part_cap-1 */
static seq_t *partitions = NULL;
static idx_t part_cap = 0;

/* Ensure that partitions[0..n] are computed and stored. */
static void ensure_partitions(idx_t n)
{
    /* Nothing to do if already computed */
    if (part_cap > n)
        return;

    idx_t old_cap = part_cap;
    idx_t new_cap = n + 1;   /* we need entries 0 .. n inclusive */

    if (part_cap == 0) {
        /* First allocation */
        partitions = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        if (!partitions)
            return;   /* allocation failure – caller will handle zero */

#ifdef __cplusplus
        /* Construct first element p(0) = 1 */
        new (partitions) seq_t(1);
#else
        partitions[0] = (seq_t)1;
#endif
    } else {
        /* Grow the array */
        seq_t *tmp = (seq_t *)realloc(partitions,
                                      (size_t)new_cap * sizeof(seq_t));
        if (!tmp)
            return;   /* allocation failure */

        partitions = tmp;

#ifdef __cplusplus
        /* Construct any new elements (initially zero). */
        for (idx_t i = old_cap; i < new_cap; ++i)
            new (partitions + i) seq_t(0);
#endif
    }

    /* Compute partition numbers from the first new index up to n. */
    idx_t start = old_cap;
    if (old_cap == 0)
        start = 1;   /* p(0) is already 1, do not recompute it */

    for (idx_t i = start; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (idx_t k = 1; ; ++k) {
            /* Generalised pentagonal numbers */
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i)
                break;
            seq_t term = partitions[i - pent1];
            if (k & 1)
                pos += term;
            else
                neg += term;

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i)
                continue;
            term = partitions[i - pent2];
            if (k & 1)
                pos += term;
            else
                neg += term;
        }
        partitions[i] = pos - neg;
    }

    part_cap = new_cap;
}

/* External function implementing A027337 */
seq_t a(idx_t n)
{
    ensure_partitions(n);
    seq_t res = partitions[n];
    if (n >= 3)
        res -= partitions[n - 3];
    return res;
}
