/*
 * A213193  O.g.f.: Sum_{n>=0} (4*n+1)^(4*n+1) * exp(-(4*n+1)^4*x) * x^n / n!.
 * https://oeis.org/A213193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/n! * Sum_{k=0..n} (-1)^(n-k) * binomial(n,k) *
 *            (4*k+1)^(4*n+1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   my_big_pow, my_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This file relies only on the standard integer types and the
   integer type seq_t/idx_t defined by the build environment. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/my_big_pow.h"
#include "../utils/my_factorial.h"

/* Minimal internal helpers (do not use the utilities provided in the
   build environment).  They work with the types seq_t and idx_t.  */

/* Compute binomial coefficient C(n,k) without large intermediate values. */
static seq_t my_binomial(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - i + 1) / (seq_t)i;
    }
    return res;
}

/* Compute the nth term of sequence A213193. */
seq_t a(idx_t n) {
    seq_t exp = (seq_t)(4 * (seq_t)n) + (seq_t)1;
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t base = (seq_t)(4 * (seq_t)k) + (seq_t)1;
        seq_t power = my_big_pow(base, exp);
        seq_t coeff = my_binomial(n, k);
        seq_t term = coeff * power;
        if (((n - k) & 1u) != 0) {
            sum_neg += term;
        } else {
            sum_pos += term;
        }
    }

    seq_t numerator = sum_pos - sum_neg;   /* numerator is non‑negative */
    seq_t fact = my_factorial(n);
    return numerator / fact;
}
