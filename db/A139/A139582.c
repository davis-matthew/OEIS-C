/*
 * A139582  Twice partition numbers.
 * https://oeis.org/A139582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*A000041(n) (twice the partition number)
 * Range:     u8 n <= 13, u16 n <= 39, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of partition numbers */
    static seq_t *p = NULL;
    static idx_t  sz = 0;          /* allocated length */
    static idx_t  max = 0;         /* highest n for which p[n] is known */

    /* initialize cache */
    if (sz == 0) {
        /* allocate at least one element for p[0] */
        p = (seq_t *)malloc(sizeof(seq_t));
        if (!p) abort();
        p[0] = (seq_t)1;   /* p(0) = 1 */
        sz = 1;
        max = 0;
    }

    /* grow the cache if necessary */
    if (n + 1 > sz) {
        seq_t *tmp = (seq_t *)realloc(p, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        p = tmp;
        sz = (idx_t)(n + 1);
    }

    /* compute partition numbers up to n */
    for (idx_t i = max + 1; i <= n; ++i) {
        long long sum = 0;   /* use signed intermediate */
        for (idx_t k = 1; ; ++k) {
            /* generalized pentagonal numbers */
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            if (k & 1)       /* odd k: +p(i-pent1) */
                sum += (long long)p[i - pent1];
            else             /* even k: -p(i-pent1) */
                sum -= (long long)p[i - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 <= i) {
                if (k & 1)
                    sum += (long long)p[i - pent2];
                else
                    sum -= (long long)p[i - pent2];
            }
        }
        p[i] = (seq_t)sum;
    }
    max = n;

    /* return twice the partition number */
    return (seq_t)p[n] * (seq_t)2;
}
