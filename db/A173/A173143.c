/*
 * A173143  Partial sums of the squarefree numbers, A005117.
 * https://oeis.org/A173143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=1}^{n} A005117(k) – sum of the first n squarefree
 *            numbers
 * Range:     u8 n <= 18, u16 n <= 283, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

/* Cached cumulative sums of squarefree numbers.  sum[1] holds a(1). */
static seq_t *sum = NULL;          /* array of cumulative sums (1‑based) */
static idx_t  sum_cap = 0;         /* allocated capacity (number of entries) */
static idx_t  max_cached = 0;     /* highest n that has been computed */
static idx_t  next_test = 1;      /* next integer to test for squarefreeness */

/* Ensure the sum array has space for at least 'need' entries (1‑based). */
static void
grow_sum(idx_t need)
{
    if (sum_cap >= need)
        return;
    idx_t new_cap = sum_cap ? sum_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)malloc(sizeof(seq_t) * new_cap);
    if (!tmp) abort();                /* fatal on allocation failure */
    if (sum)
        memcpy(tmp, sum, sizeof(seq_t) * sum_cap);
    free(sum);
    sum = tmp;
    sum_cap = new_cap;
}

/* Main sequence routine. */
seq_t
a(idx_t n)
{
    if (n == 0)            /* offset 1 – return 0 for safety */
        return (seq_t)0;

    /* Grow sum array if necessary */
    grow_sum(n + 1);

    /* Generate additional terms until we have a(n) */
    while (max_cached < n)
    {
        /* Find next squarefree integer */
        while (true)
        {
            if (is_squarefree((seq_t)next_test))
                break;
            ++next_test;
        }

        /* Compute new cumulative sum */
        seq_t val = (seq_t)next_test;
        seq_t new_sum = (max_cached == 0) ? val : sum[max_cached] + val;
        ++max_cached;
        sum[max_cached] = new_sum;
        ++next_test;   /* advance for next search */
    }

    return sum[n];
}
