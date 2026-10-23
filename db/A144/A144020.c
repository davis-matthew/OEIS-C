/*
 * A144020  Numbers of the form 1+i^2+j^2+k^2 with 1 <= i <= j <= k.
 * https://oeis.org/A144020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth distinct integer of the form 1 + i² + j² + k²
 *            with 1 ≤ i ≤ j ≤ k, sorted increasingly. The implementation
 *            builds the list incrementally, re‑computing all triples whenever
 *            the bound is enlarged, then sorts and removes duplicates. It
 *            works for any unsigned integer type seq_t (8–128 b...
 * Range:     u8 none, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* a comparison function for qsort that works with any seq_t type  */
static int seq_compare(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return  1;
    return 0;
}

/*--- static state for memoisation ------------------------------------*/
static idx_t  maxK      = 0;   /* current upper bound for i,j,k          */
static size_t uniq_cnt  = 0;   /* number of distinct values found       */
static seq_t *values    = NULL;/* array holding the sorted uniques       */

/* Rebuild the array of sums for the current bound maxK   */
static void rebuild(void)
{
    /* estimated number of triples (i≤j≤k) */
    size_t tot_triples = (size_t)(maxK + 2) * (maxK + 1) * maxK / 6;
    seq_t *tmp = (seq_t *)malloc(tot_triples * sizeof(seq_t));
    if (!tmp) {
        /* out of memory – abort program (allowed in this context) */
        abort();
    }

    size_t pos = 0;
    for (idx_t i = 1; i <= maxK; ++i) {
        seq_t ii = (seq_t)i * i;
        for (idx_t j = i; j <= maxK; ++j) {
            seq_t jj = (seq_t)j * j;
            for (idx_t k = j; k <= maxK; ++k) {
                seq_t kk = (seq_t)k * k;
                tmp[pos++] = (seq_t)1 + ii + jj + kk;
            }
        }
    }

    /* Sort the raw list of sums */
    qsort(tmp, pos, sizeof(seq_t), seq_compare);

    /* Remove duplicates in‑place */
    size_t uniq = 0;
    for (size_t t = 0; t < pos; ++t) {
        if (t == 0 || tmp[t] != tmp[t - 1]) {
            tmp[uniq++] = tmp[t];
        }
    }

    free(values);
    values   = tmp;
    uniq_cnt = uniq;
}

/*--- public function -------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* undefined for n < 1 */

    /* If we don't have enough values, grow the bound */
    if (n > uniq_cnt) {
        /* Initialise bound on first call */
        if (maxK == 0)
            maxK = 20;     /* modest start, will grow quickly */

        while (n > uniq_cnt) {
            /* increase maxK by ~50 % each step */
            maxK = (maxK * 3) / 2 + 1;
            rebuild();
        }
    }

    /* Return the n‑th value (1‑based) */
    return values[n - 1];
}
