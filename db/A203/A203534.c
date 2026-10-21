/*
 * A203534  G.f.: exp( Sum_{n>=1} sigma(n)*A002203(n)*x^n/n ) where A002203 is
 *          the companion Pell numbers.
 * https://oeis.org/A203534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = (1/n) * sum_{k=1..n} sigma(k) * (2*Q_k) *
 *            a(n-k), where sigma(k) is the sum of divisors of k and Q_k is
 *            the companion Pell number (Q_1 = 1, Q_2 = 3, Q_n =
 *            2*Q_{n-1}+Q_{n-2}). The factor 2*Q_k equals A002203(k) used in
 *            the definition
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static caches */
static seq_t *sigma_arr = NULL;   /* sum of divisors, indices start at 1 */
static idx_t  sigma_size = 0;

static seq_t *comp_arr = NULL;    /* 2*companion Pell numbers, indices start at 1 */
static idx_t  comp_size  = 0;

static seq_t *a_arr = NULL;      /* sequence values */
static idx_t  a_size   = 0;

/* ------------------------------------------------------------------ */
/* Compute sigma[1..n] (sum of divisors) */
static void compute_sigma(idx_t n)
{
    if (n <= sigma_size)
        return;

    seq_t *new_arr = (seq_t *)realloc(sigma_arr, (n + 1) * sizeof(seq_t));
    if (!new_arr)
        return;                    /* out of memory – abort silently */
    sigma_arr = new_arr;

    /* initialise to zero */
    for (idx_t i = 0; i <= n; ++i)
        sigma_arr[i] = 0;

    for (idx_t d = 1; d <= n; ++d) {
        for (idx_t m = d; m <= n; m += d)
            sigma_arr[m] += d;
    }
    sigma_size = n;
}

/* ------------------------------------------------------------------ */
/* Compute 2 * companion Pell numbers, indices start at 1 */
static void compute_comp(idx_t n)
{
    if (n <= comp_size)
        return;

    seq_t *new_arr = (seq_t *)realloc(comp_arr, (n + 1) * sizeof(seq_t));
    if (!new_arr)
        return;
    comp_arr = new_arr;

    for (idx_t i = 1; i <= n; ++i) {
        if (i == 1)
            comp_arr[i] = (seq_t)2;          /* 2*Q_1 = 2*1 */
        else if (i == 2)
            comp_arr[i] = (seq_t)6;          /* 2*Q_2 = 2*3 */
        else
            comp_arr[i] = comp_arr[i - 1] * 2 + comp_arr[i - 2];
    }
    comp_size = n;
}

/* ------------------------------------------------------------------ */
/* Compute a[0..n] using the recurrence */
static void compute_a(idx_t n)
{
    if (n <= a_size)
        return;

    seq_t *new_arr = (seq_t *)realloc(a_arr, (n + 1) * sizeof(seq_t));
    if (!new_arr)
        return;
    a_arr = new_arr;

    if (a_size == 0)            /* initialise a[0] */
        a_arr[0] = (seq_t)1;

    for (idx_t i = a_size + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = sigma_arr[k] * comp_arr[k];
            term *= a_arr[i - k];
            sum += term;
        }
        a_arr[i] = sum / (seq_t)i;   /* exact division */
    }
    a_size = n;
}

/* ------------------------------------------------------------------ */
/* Public function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) {                     /* ensure a[0] exists */
        if (a_size == 0) {
            a_arr = (seq_t *)calloc(1, sizeof(seq_t));
            a_arr[0] = (seq_t)1;
            a_size = 0;
        }
        return a_arr[0];
    }

    compute_sigma(n);
    compute_comp(n);
    compute_a(n);

    return a_arr[n];
}
