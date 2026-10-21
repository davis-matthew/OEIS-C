/*
 * A222735  Total sum of parts of multiplicity 7 in all partitions of n.
 * https://oeis.org/A222735
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{t≥1, 7t≤n} t·(p(n‑7t) – p(n‑8t)) , where p(k) is the
 *            number of partitions of k (with the convention p(k)=0 for k<0).
 *            The sequence starts at n=7
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 136, u64 n <= 413,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* calloc, free, memcpy */

/* Cached partition numbers */
static seq_t *partitions = NULL;
static idx_t part_max = 0;

/* Ensure that partitions[0…n] are computed */
static void ensure_partitions(idx_t n)
{
    if (n <= part_max)
        return;

    size_t newsize = (size_t)(n + 1);
    seq_t *newp = (seq_t *)calloc(newsize, sizeof(seq_t));
    if (!newp)                /* out of memory – keep old cache, caller gets 0 */
        return;

    /* copy previous data */
    if (partitions)
    {
        memcpy(newp, partitions, (size_t)(part_max + 1) * sizeof(seq_t));
        free(partitions);
    }

    newp[0] = (seq_t)1;       /* p(0)=1 */

    /* compute p(i) for i = part_max+1 … n */
    for (idx_t i = part_max + 1; i <= n; ++i)
    {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        int sign = 1;

        for (idx_t k = 1; ; ++k)
        {
            idx_t g1 = ((idx_t)k * (3 * (idx_t)k - 1)) / 2;
            if (g1 > i)
                break;
            if (sign > 0)
                pos += newp[i - g1];
            else
                neg += newp[i - g1];

            idx_t g2 = ((idx_t)k * (3 * (idx_t)k + 1)) / 2;
            if (g2 <= i)
            {
                if (sign > 0)
                    pos += newp[i - g2];
                else
                    neg += newp[i - g2];
            }
            sign = -sign;
        }
        newp[i] = pos - neg;   /* pos ≥ neg */
    }

    partitions = newp;
    part_max   = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 7)
        return (seq_t)0;      /* below offset – undefined behaviour */

    ensure_partitions(n);

    seq_t sum = (seq_t)0;

    for (idx_t t = 1; 7 * t <= n; ++t)
    {
        idx_t i1 = n - 7 * t;
        seq_t diff = partitions[i1];

        if (n >= 8 * t)
        {
            idx_t i2 = n - 8 * t;
            diff -= partitions[i2];   /* p(n‑7t) – p(n‑8t) */
        }

        sum += (seq_t)t * diff;
    }

    return sum;
}
