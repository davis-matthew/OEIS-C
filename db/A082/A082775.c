/*
 * A082775  Convolution of natural numbers >= 2 and the partition numbers
 *          (A000041).
 * https://oeis.org/A082775
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0}^{n-2} (n-k) * p(k) p(k)=A000041(k)
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A082775:  a(2)=2, a(n)=a(n-1)+Σ_{k=0}^{n-3} p(k)+2*p(n-2) for n>2  */

#include <stdlib.h>
#include <stdbool.h>

/* Static state for memoisation */
static seq_t *p      = NULL;      /* partition numbers p[0..cur_max]   */
static seq_t *sumP   = NULL;      /* prefix sums of p[0..cur_max]      */
static seq_t *a_arr  = NULL;      /* sequence values a[0..cur_max]     */
static idx_t cur_max = 0;          /* largest n that has been computed  */

/* Extend the tables so that indices 0..n are valid */
static void ensure_up_to(idx_t n)
{
    if (n <= cur_max) return;            /* nothing to do */

    size_t new_size = (size_t)n + 1;

    /* Reallocate the arrays */
    p     = (seq_t *)realloc(p,     new_size * sizeof(seq_t));
    sumP  = (seq_t *)realloc(sumP,  new_size * sizeof(seq_t));
    a_arr = (seq_t *)realloc(a_arr, new_size * sizeof(seq_t));

    /* Initialise new elements */
    for (idx_t i = cur_max + 1; i <= n; ++i) {
        p[i]    = 0;
        sumP[i] = 0;
        a_arr[i] = 0;
    }

    /* Base case: p[0] = 1, sumP[0] = 1.  Set only once. */
    if (cur_max == 0) {
        p[0]    = 1;
        sumP[0] = 1;
    }

    /* Compute partition numbers p[i] for i = cur_max+1 .. n  */
    for (idx_t i = cur_max + 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        bool addPositive = true;               /* sign for the pair of pentagonal numbers */

        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;

            if (addPositive) pos += p[i - pent1];
            else             neg += p[i - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 <= i) {
                if (addPositive) pos += p[i - pent2];
                else             neg += p[i - pent2];
            }
            addPositive = !addPositive;
        }
        p[i] = pos - neg;                       /* pos ≥ neg by definition */
    }

    /* Compute prefix sums of partitions */
    for (idx_t i = cur_max + 1; i <= n; ++i) {
        sumP[i] = sumP[i - 1] + p[i];
    }

    /* Compute the sequence values */
    for (idx_t i = cur_max + 1; i <= n; ++i) {
        if (i == 2)                        /* base value */
            a_arr[i] = 2;
        else if (i >= 3)                    /* recurrence */
            a_arr[i] = a_arr[i - 1] + sumP[i - 3] + (seq_t)2 * p[i - 2];
        /* i < 2: a_arr not used */
    }

    cur_max = n;                            /* new maximum achieved */
}

/* Public function required by the harness */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                    /* undefined below the offset */
    ensure_up_to(n);
    return a_arr[n];
}
