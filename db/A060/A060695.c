/*
 * A060695  a(n) = gcd(2n, A060766(2n)).
 * https://oeis.org/A060695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(2n, lcm of the differences between consecutive sorted
 *            divisors of 2n)
 * Range:     u8 n <= 18, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* comparison function for qsort */
static int cmp_idx(const void *a, const void *b)
{
    idx_t va = *(const idx_t *)a;
    idx_t vb = *(const idx_t *)b;
    return (va > vb) - (va < vb);
}

/**
 * Compute A060695(a(n)) for n >= 1.
 */
seq_t a(idx_t n)
{
    /* m = 2 * n */
    seq_t m = (seq_t)n * 2;
    idx_t M = (idx_t)m;              /* M fits into idx_t for n <= 10000 (m <= 20000) */

    /* collect all divisors of M */
    idx_t divs[300];
    size_t div_cnt = 0;

    /* integer sqrt of M */
    idx_t sqrtM = 0;
    while ((sqrtM + 1) * (sqrtM + 1) <= M)
        ++sqrtM;

    for (idx_t i = 1; i <= sqrtM; ++i)
    {
        if ((m % (seq_t)i) == 0)
        {
            divs[div_cnt++] = i;
            idx_t other = (idx_t)(m / (seq_t)i);
            if (other != i)
                divs[div_cnt++] = other;
        }
    }

    /* sort divisors */
    qsort(divs, div_cnt, sizeof(idx_t), cmp_idx);

    /* compute lcm of consecutive differences */
    seq_t lcm = 1;
    for (size_t j = 1; j < div_cnt; ++j)
    {
        seq_t diff = (seq_t)(divs[j] - divs[j - 1]);   /* diff > 0 */
        if (diff == 0)
            continue;
        seq_t g = gcd(lcm, diff);
        lcm = (lcm / g) * diff;                       /* lcm = lcm(lcm, diff) */
    }

    /* final result */
    return gcd(m, lcm);
}
