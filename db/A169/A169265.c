/*
 * A169265  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 12, a(2) = 132, ... with the generating
 *            function A(t) = (t^29 + 2*t^28 + … + 2*t + 1)/(55*t^29 - 10*t^28
 *            - … - 10*t - 1). The numerator and denominator give the linear
 *            recurrence a(n) = 10·(a(n-1)+…+a(n-28)) - 55·a(n-29) for n ≥ 29
 *            with the initial values for 0 ≤ n ≤ 28 obtained...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *seq = NULL;          /* cached sequence values */
static size_t cap = 0;             /* allocated capacity of 'seq' */
static idx_t  computed = 0;        /* next index to compute (size of initialized part) */
static seq_t  sum_last28 = 0;      /* sum of the last 28 computed terms (for n ≥ 29) */

/* Ensure that the sequence array can hold indices up to n */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) <= cap) return;
    size_t new_cap = (n + 1) * 2;
    if (new_cap < 128u) new_cap = 128u;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq = tmp;
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < computed) return seq[n];

    /* First time: compute the initial values up to min(n,28) */
    if (computed == 0) {
        ensure_cache(n);
        seq[0] = (seq_t)1;
        computed = 1;
        if (n == 0) return seq[0];
        for (idx_t i = 1; i <= 28 && i <= n; ++i) {
            seq_t sum_prev = 0;
            for (idx_t j = 0; j < i; ++j) sum_prev += seq[j];
            seq[i] = (seq_t)2 + (seq_t)10 * sum_prev;
            ++computed;
        }
        /* If we haven't computed up to index 28 yet and n<29, return early */
        if (computed <= 28 && n < 29) return seq[n];
    }
    /* If we have computed some but less than 29 and n ≥ 29,
       finish computing the base up to index 28. */
    else if (computed <= 28 && n >= 29) {
        for (idx_t i = computed; i <= 28; ++i) {
            seq_t sum_prev = 0;
            for (idx_t j = 0; j < i; ++j) sum_prev += seq[j];
            seq[i] = (seq_t)2 + (seq_t)10 * sum_prev;
            ++computed;
        }
    }

    /* If we just finished the base (i.e. computed == 29 now),
       compute sum_last28 = sum_{i=1}^{28} seq[i] */
    if (computed == 29) {
        sum_last28 = 0;
        for (idx_t i = 1; i <= 28; ++i) sum_last28 += seq[i];
    }

    /* Compute remaining values up to n using the recurrence */
    for (idx_t i = computed; i <= n; ++i) {
        if (i <= 28) {
            /* This case happens only when computed was 0 or <29
               and n < 29 – but we already handled those above. */
            seq_t sum_prev = 0;
            for (idx_t j = 0; j < i; ++j) sum_prev += seq[j];
            seq[i] = (seq_t)2 + (seq_t)10 * sum_prev;
        } else {
            /* i >= 29 */
            seq_t val = (seq_t)10 * sum_last28 - (seq_t)55 * seq[i - 29];
            seq[i] = val;
            sum_last28 += seq[i] - seq[i - 28];
        }
        ++computed;
    }
    return seq[n];
}
