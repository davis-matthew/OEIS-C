/*
 * A061121  Number of degree-n permutations of order exactly 6.
 * https://oeis.org/A061121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of permutations on n symbols whose order is
 *            exactly 6. Computed via inclusion–exclusion on cycle types: a(n)
 *            = total(n) – no3(n) – no2(n) + 1 where total(n) – permutations
 *            with cycles of lengths {1,2,3,6} no3(n) – permutations with
 *            cycles of lengths {1,2} no2(n) – permutations with...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort, size_t */

static seq_t *total   = NULL;   /* allowed lengths {1,2,3,6}   */
static seq_t *no2only = NULL;   /* allowed lengths {1,2}      */
static seq_t *no3only = NULL;   /* allowed lengths {1,3}      */
static size_t max_cal = 0;      /* largest n for which tables are filled */

static void ensure_capacity(idx_t n)
{
    if (n <= (idx_t)max_cal)
        return;   /* already computed */

    size_t new_cap = (size_t)n + 1;   /* include index n */

    /* Allocate or grow total */
    seq_t *tmp;
    if (total == NULL) {
        tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) abort();
        total = tmp;
    } else {
        tmp = (seq_t *)realloc(total, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        total = tmp;
    }

    /* Allocate or grow no2only */
    if (no2only == NULL) {
        tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) abort();
        no2only = tmp;
    } else {
        tmp = (seq_t *)realloc(no2only, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        no2only = tmp;
    }

    /* Allocate or grow no3only */
    if (no3only == NULL) {
        tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) abort();
        no3only = tmp;
    } else {
        tmp = (seq_t *)realloc(no3only, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        no3only = tmp;
    }

    /* Initialise the new entries */
    if (max_cal == 0) {
        total[0]   = (seq_t)1;
        no2only[0] = (seq_t)1;
        no3only[0] = (seq_t)1;
    }

    /* Compute new entries incrementally */
    for (idx_t i = max_cal + 1; i <= n; ++i) {
        seq_t sum_total   = 0;
        seq_t sum_no2only = 0;
        seq_t sum_no3only = 0;

        /* ---------- total (S = {1,2,3,6}) ---------- */
        /* k = 1 */
        sum_total += total[i - 1];
        /* k = 2 */
        if (i >= 2)
            sum_total += (seq_t)(i - 1) * total[i - 2];
        /* k = 3 */
        if (i >= 3) {
            seq_t fac = (seq_t)(i - 1) * (seq_t)(i - 2);
            sum_total += fac * total[i - 3];
        }
        /* k = 6 */
        if (i >= 6) {
            seq_t fac = (seq_t)(i - 1) * (seq_t)(i - 2) *
                        (seq_t)(i - 3) * (seq_t)(i - 4) * (seq_t)(i - 5);
            sum_total += fac * total[i - 6];
        }
        total[i] = sum_total;

        /* ---------- no2only (S = {1,2}) ---------- */
        /* k = 1 */
        sum_no2only += no2only[i - 1];
        /* k = 2 */
        if (i >= 2)
            sum_no2only += (seq_t)(i - 1) * no2only[i - 2];
        no2only[i] = sum_no2only;

        /* ---------- no3only (S = {1,3}) ---------- */
        /* k = 1 */
        sum_no3only += no3only[i - 1];
        /* k = 3 */
        if (i >= 3) {
            seq_t fac = (seq_t)(i - 1) * (seq_t)(i - 2);
            sum_no3only += fac * no3only[i - 3];
        }
        no3only[i] = sum_no3only;
    }

    max_cal = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* offset is 1 */

    ensure_capacity(n);

    seq_t result = total[n];
    result -= no2only[n];
    result -= no3only[n];
    result += (seq_t)1;   /* identity permutation */

    return result;
}
