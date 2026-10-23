/*
 * A103650  G.f.: x^2/((1-x^2)^2*Product_{i>0}(1-x^i)).
 * https://oeis.org/A103650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1}^{⌊n/2⌋} k * p(n-2k) where p(m) is the partition
 *            number of m. For partitions, p(0)=1 and p(m)=0 for m<0. This
 *            function computes the sequence A103650 using the pentagonal
 *            number recurrence for partitions and then applies the above sum
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 102, u64 n <= 352,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *partitions = NULL;
static idx_t partitions_cap = 0;

static void compute_partitions(idx_t max_n)
{
    if (partitions_cap > max_n)
        return;

    seq_t *tmp = (seq_t *)realloc(partitions, (max_n + 1) * sizeof(seq_t));
    if (!tmp && (max_n + 1) > 0)
        abort();
    partitions = tmp;
    partitions_cap = max_n + 1;

    partitions[0] = 1;
    for (idx_t n = 1; n <= max_n; ++n)
    {
        seq_t pos = 0;
        seq_t neg = 0;
        for (idx_t j = 1; ; ++j)
        {
            idx_t pent1 = j * (3 * j - 1) / 2;
            if (pent1 > n)
                break;
            if (j & 1)
                pos += partitions[n - pent1];
            else
                neg += partitions[n - pent1];

            idx_t pent2 = j * (3 * j + 1) / 2;
            if (pent2 > n)
                continue;
            if (j & 1)
                pos += partitions[n - pent2];
            else
                neg += partitions[n - pent2];
        }
        /* pos ≥ neg by theory of the recurrence */
        partitions[n] = pos - neg;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* defined only for n ≥ 1 in OEIS, but harmless */

    compute_partitions(n);

    seq_t res = 0;
    for (idx_t k = 1; k <= n / 2; ++k)
    {
        seq_t term = (seq_t)k * partitions[n - 2 * k];
        res += term;
    }
    return res;
}
