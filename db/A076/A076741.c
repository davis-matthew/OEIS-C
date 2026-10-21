/*
 * A076741  Nonzero coefficients of the polynomials in the numerator of
 *          1/(1+x^2) and its successive derivatives, starting with the
 *          constant term.
 * https://oeis.org/A076741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, easy
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 32, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   total_terms
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A076741
 *  Nonzero coefficients of the polynomials in the numerator of 1/(1+x^2)
 *  and its successive derivatives, starting with the constant term.
 *
 *  Formula:
 *      a(n) = (-1)^((d + k)/2) * d! * binom(d + 1, k)
 *  where (d, k) is the derivative order d and the degree k of the
 *  coefficient, enumerated by increasing d and, for each d, increasing k
 *  with the same parity as d (k = 0,2,4,… for even d and k = 1,3,5,… for odd d).
 *
 *  The code below computes a(n) directly for any index n >= 0.
 */

#include <stdint.h>
#include "../utils/total_terms.h"

seq_t a(idx_t idx) {
    /* 1. Find derivative order d such that total_terms(d-1) <= idx < total_terms(d) */
    idx_t low = 0;
    idx_t high = 1;
    while (total_terms(high) <= idx) {
        high <<= 1;                   /* grow high until it exceeds idx */
    }
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (total_terms(mid) > idx)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;                     /* derivative order */

    /* 2. Offset within this derivative (0‑based) */
    idx_t prev = (d == 0) ? 0 : total_terms(d - 1);
    idx_t offset = idx - prev;

    /* 3. Compute the degree k */
    idx_t first_k = (d & 1) ? 1 : 0;    /* first non‑zero degree for this derivative */
    idx_t k = first_k + (offset << 1);   /* k = first_k + 2*offset */

    /* 4. Sign factor (-1)^((d+k)/2) */
    idx_t exp = (d + k) >> 1;           /* (d+k)/2, integer because d+k is even */
    seq_t sign = (exp & 1) ? -1 : 1;

    /* 5. Factorial d! (computed in __int128 and cast to seq_t) */
    seq_t fact = 1;
    for (idx_t i = 2; i <= d; ++i) {
        fact = (seq_t)(((__int128)fact * i));
    }

    /* 6. Binomial coefficient C(d+1, k) */
    seq_t binom = 1;
    for (idx_t i = 1; i <= k; ++i) {
        binom = (seq_t)(((__int128)binom * (d + 2 - i)) / i);
    }

    /* 7. Result: sign * fact * binom */
    seq_t result = (seq_t)(((__int128)sign * binom * fact));

    return result;
}
