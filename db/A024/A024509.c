/*
 * A024509  Numbers that are the sum of 2 nonzero squares, including
 *          repetitions.
 * https://oeis.org/A024509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth number that can be written as a^2 + b^2 with * a,
 *            b ≥ 1, counting a number each time it has a distinct unordered *
 *            representation (a ≤ b). This includes repeated values when the
 *            number * has multiple such pairs
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Comparator for qsort over seq_t */
static int cmp_seq(const void *p, const void *q)
{
    const seq_t *x = (const seq_t *)p;
    const seq_t *y = (const seq_t *)q;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Return the nth term of OEIS A024509 */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n ≤ 0 */

    /* Roughly estimate the search limit to cover at least n pairs.
     * The density of unordered pairs is about 0.39, so 3n is a safe
     * initial bound for n up to tens of thousands. */
    idx_t limit = n * 3;
    if (limit < 50)
        limit = 50;

    while (1) {
        /* Determine the largest a such that a^2 ≤ limit */
        idx_t max_a = 0;
        while ((max_a + 1) * (max_a + 1) <= limit)
            ++max_a;

        /* Allocate a buffer large enough for the expected number of pairs.
         * We grow it dynamically as needed. */
        size_t cap = (size_t)(0.4 * limit) + 10;   /* 0.4 is an upper bound */
        seq_t *sums = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!sums)
            return 0;                     /* allocation failure */

        size_t cnt = 0;

        /* Enumerate unordered pairs (a, b) with 1 ≤ a ≤ b and a^2 + b^2 ≤ limit */
        for (idx_t a = 1; a <= max_a; ++a) {
            idx_t a2 = a * a;
            for (idx_t b = a; b <= max_a; ++b) {
                idx_t sum = a2 + b * b;
                if (sum > limit)
                    break;

                /* Grow buffer if needed */
                if (cnt == cap) {
                    cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(sums, cap * sizeof(seq_t));
                    if (!tmp) {
                        free(sums);
                        return 0;
                    }
                    sums = tmp;
                }
                sums[cnt++] = (seq_t)sum;
            }
        }

        if (cnt >= n) {
            /* Sort and pick the nth element (1‑based). */
            qsort(sums, cnt, sizeof(seq_t), cmp_seq);
            seq_t result = sums[n - 1];
            free(sums);
            return result;
        }

        /* Not enough terms: double the search limit and retry. */
        free(sums);
        limit <<= 1;
    }
}
