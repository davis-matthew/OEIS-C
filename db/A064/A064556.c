/*
 * A064556  Ramanujan's tau function (or tau numbers (A000594)) for 10^n.
 * https://oeis.org/A064556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = tau(10^n) = tau(2^n) * tau(5^n) where tau(p^0)=1,
 *            tau(p^1)=tau(p), tau(p^{k+1}) = tau(p) * tau(p^k) - p^{11} *
 *            tau(p^{k-1})
 * Range:     i8 n <= 0, i16 n <= 0, i32 n <= 1, i64 n <= 3, i128 n <= 7,
 *            big any n
 * Validated: standard, pass, 22 terms through n = 176 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Compute values of tau(p^k) for k = 0..n. */
static void compute_tau(idx_t n, seq_t **arr, size_t *cap, idx_t *max,
                        seq_t tau_p, seq_t pow_p11)
{
    /* Allocate initial array if necessary. */
    if (*arr == NULL) {
        size_t c = (size_t)n + 1;
        if (c < 8) c = 8;               /* minimum small size */
        *arr = mem_new(seq_t, c);
        if (*arr == NULL) abort();
        (*arr)[0] = (seq_t)1;           /* tau(p^0) = 1 */
        *cap = c;
        *max = 0;
    }

    /* Grow arrays to length at least n+1. */
    if (*cap <= (size_t)n) {
        size_t new_cap = *cap ? *cap * 2 : 8;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }

    /* Initialize first prime power if required. */
    if (*max == 0 && n >= 1) {
        (*arr)[1] = tau_p;              /* tau(p^1) = tau(p) */
        *max = 1;
    }

    /* Fill missing values using the recurrence. */
    for (idx_t k = *max + 1; k <= n; ++k) {
        seq_t prev1 = (*arr)[k - 1];
        seq_t prev2 = (*arr)[k - 2];
        (*arr)[k] = tau_p * prev1 - pow_p11 * prev2;
    }
    *max = n;
}

/* Sequence entry a(n) = tau(10^n). */
seq_t a(idx_t n)
{
    /* Static storage for values of tau(2^k) and tau(5^k). */
    static seq_t *tau2 = NULL, *tau5 = NULL;
    static size_t cap2 = 0, cap5 = 0;
    static idx_t max2 = 0, max5 = 0;

    /* Constants for the primes 2 and 5. */
    const seq_t tau_2   = (seq_t)-24;          /* tau(2) */
    const seq_t tau_5   = (seq_t)4830;         /* tau(5) */
    const seq_t pow2_11 = (seq_t)2048;         /* 2^11  */
    const seq_t pow5_11 = (seq_t)48828125;     /* 5^11  */

    /* Ensure prime power arrays up to n are computed. */
    compute_tau(n, &tau2, &cap2, &max2, tau_2, pow2_11);
    compute_tau(n, &tau5, &cap5, &max5, tau_5, pow5_11);

    /* Return the product tau(2^n)*tau(5^n) = tau(10^n). */
    return tau2[n] * tau5[n];
}
