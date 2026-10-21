/*
 * A222732  Total sum of parts of multiplicity 4 in all partitions of n.
 * https://oeis.org/A222732
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1} k * (p(n-4k) – p(n-5k)) where p(m) is the
 *            partition function (p(0)=1, p(m)=0 for m<0)
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 122, u64 n <= 389,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;          /* offset not defined, return 0 */

    idx_t N = n;
    seq_t *p = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!p)
        return 0;          /* allocation failed */

    /* initialize partition array */
    for (idx_t i = 0; i <= N; ++i)
        p[i] = 0;
    p[0] = 1;

    /* dynamic programming: p[j] += p[j - i]  for all i ≤ j */
    for (idx_t i = 1; i <= N; ++i)
    {
        for (idx_t j = i; j <= N; ++j)
        {
            p[j] += p[j - i];
        }
    }

    /* compute the sum of parts of multiplicity 4 */
    seq_t sum1 = 0, sum2 = 0;
    for (idx_t k = 1; 4 * k <= N; ++k)
        sum1 += (seq_t)k * p[N - 4 * k];
    for (idx_t k = 1; 5 * k <= N; ++k)
        sum2 += (seq_t)k * p[N - 5 * k];

    seq_t result = sum1 - sum2;  /* guaranteed non‑negative */

    free(p);
    return result;
}
