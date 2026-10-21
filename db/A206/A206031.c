/*
 * A206031  a(n) = product of numbers k <= sigma(n) such that k = sigma(d) for
 *          any divisor d of n where sigma = A000203.
 * https://oeis.org/A206031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏ { σ(d) | d divides n } over distinct values of σ(d)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 47, u64 n <= 119, u128 n <= 479,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (σ = sum of divisors, A000203) */

#include <stdlib.h>
#include "../utils/sigma_of.h"

/* Helper: compare two seq_t values for qsort; must not be named seq_cmp. */
static int seq_t_compare(const void *p, const void *q)
{
    const seq_t *pa = (const seq_t *)p;
    const seq_t *qa = (const seq_t *)q;
    if (*pa < *qa) return -1;
    if (*pa > *qa) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety, offset is 1 */

    /* allocate space for all divisors of n (worst case n divisors) */
    seq_t *vals = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!vals) return 0;                 /* allocation failure */

    size_t cnt = 0;
    for (idx_t d = 1; d <= n; ++d)
    {
        if (n % d == 0)
        {
            vals[cnt++] = sigma_of(d);
        }
    }

    /* Sort the sigma values to allow duplicate detection */
    qsort(vals, cnt, sizeof(seq_t), seq_t_compare);

    /* Multiply only distinct sigma values */
    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        if (i == 0 || seq_t_compare(&vals[i], &vals[i - 1]) != 0)
        {
            prod *= vals[i];
        }
    }

    free(vals);
    return prod;
}
