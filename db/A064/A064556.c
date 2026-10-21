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
 * Helpers:   compute_tau
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/compute_tau.h"

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
