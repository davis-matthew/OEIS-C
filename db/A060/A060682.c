/*
 * A060682  Number of distinct differences between consecutive divisors of n
 *          (ordered by size).
 * https://oeis.org/A060682
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct differences between consecutive
 *            divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static int cmp_idx(const void *a, const void *b)
{
    const idx_t *ia = (const idx_t *)a;
    const idx_t *ib = (const idx_t *)b;
    return (*ia < *ib) ? -1 : (*ia > *ib) ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* Compute floor(sqrt(n)) safely without overflow */
    idx_t limit = 0;
    for (idx_t i = 1; i <= n / i; ++i)
        ++limit;

    /* Allocate array for divisors: at most 2*limit+1 entries */
    size_t div_cap = 2 * (size_t)limit + 2;
    idx_t *divs = (idx_t *)calloc(div_cap, sizeof(idx_t));
    if (!divs)
        return 0;  /* allocation failure */

    size_t div_cnt = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        if (n % i == 0) {
            divs[div_cnt++] = i;
            idx_t j = n / i;
            if (j != i)
                divs[div_cnt++] = j;
        }
    }

    if (div_cnt < 2) {
        free(divs);
        return 0;
    }

    /* Sort divisors */
    qsort(divs, div_cnt, sizeof(idx_t), cmp_idx);

    /* Compute differences between consecutive divisors */
    size_t diff_cnt = div_cnt - 1;
    idx_t *diffs = (idx_t *)calloc(diff_cnt, sizeof(idx_t));
    if (!diffs) {
        free(divs);
        return 0;
    }

    for (size_t k = 1; k < div_cnt; ++k)
        diffs[k - 1] = divs[k] - divs[k - 1];

    free(divs);

    /* Sort differences to count distinct values */
    qsort(diffs, diff_cnt, sizeof(idx_t), cmp_idx);

    seq_t unique = 1;
    for (size_t k = 1; k < diff_cnt; ++k)
        if (diffs[k] != diffs[k - 1])
            ++unique;

    free(diffs);
    return unique;
}
