/*
 * A066186  Sum of all parts of all partitions of n.
 * https://oeis.org/A066186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = n * p(n) (p(n) = number of partitions of n)
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 88, u64 n <= 326,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t if seq_t is uint64_t */

static seq_t p[100001];          /* static storage for partition numbers */
static idx_t p_computed = 0;     /* highest n for which p[n] is known */

/* compute partition numbers p[0..n] using Euler's pentagonal recurrence */
static void compute_partitions(idx_t n)
{
    if (n < p_computed)
        return;

    /* first time: initialise the base case */
    if (p_computed == 0)
        p[0] = (seq_t)1;

    for (idx_t m = p_computed + 1; m <= n; ++m)
    {
        seq_t sumPos = 0, sumNeg = 0;
        for (idx_t k = 1; ; ++k)
        {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m)
                break;

            if (k & 1)      /* odd k -> positive sign */
                sumPos += p[m - g1];
            else            /* even k -> negative sign */
                sumNeg += p[m - g1];

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > m)
                continue;   /* still need to test the next k */

            if (k & 1)
                sumPos += p[m - g2];
            else
                sumNeg += p[m - g2];
        }
        p[m] = sumPos - sumNeg;
    }
    p_computed = n;
}

/* public function required by the programming challenge */
seq_t a(idx_t n)
{
    compute_partitions(n);          /* ensures p[n] is available */
    return p[n] * (seq_t)n;         /* a(n) = n * p(n) */
}
