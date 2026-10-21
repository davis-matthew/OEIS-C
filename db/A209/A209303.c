/*
 * A209303  Numbers of the form x^2 + SumOfSquaredDigits(x).
 * https://oeis.org/A209303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth smallest distinct value of x^2 +
 *            sum_of_squared_digits(x) for x ≥ 1
 * Range:     u8 n <= 4, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* comparator for qsort */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* return the n-th term of A209303 (1‑based index) */
seq_t a(idx_t n)
{
    /* Initial guess for how many x‑values to generate */
    idx_t limit = (n < 10 ? 20 : n * 3 + 100);

    while (1) {
        /* Allocate temporary array for f(x) values */
        seq_t *tmp = (seq_t *)malloc(limit * sizeof(seq_t));
        if (!tmp) abort();  /* insufficient memory */

        /* compute f(x) = x^2 + sum_of_squared_digits(x) for x = 1..limit */
        for (idx_t i = 0; i < limit; ++i) {
            idx_t x = i + 1;
            seq_t sq = (seq_t)x * (seq_t)x;
            seq_t sum = 0;
            for (idx_t t = x; t; t /= 10) {
                unsigned d = t % 10;
                sum += (seq_t)d * (seq_t)d;
            }
            tmp[i] = sq + sum;
        }

        /* sort the values */
        qsort(tmp, limit, sizeof(seq_t), cmp_seq_t);

        /* deduplicate */
        size_t uniq_cnt = 0;
        for (idx_t i = 0; i < limit; ++i) {
            if (uniq_cnt == 0 || tmp[i] != tmp[uniq_cnt - 1]) {
                tmp[uniq_cnt++] = tmp[i];
            }
        }

        /* if we have enough distinct terms, return the requested one */
        if (uniq_cnt >= n) {
            seq_t result = tmp[n - 1];
            free(tmp);
            return result;
        }

        /* otherwise free and expand the search range */
        free(tmp);
        limit *= 2;
    }
}
