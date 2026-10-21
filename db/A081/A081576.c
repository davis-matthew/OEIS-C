/*
 * A081576  Square array of binomial transforms of Fibonacci numbers, read by
 *          antidiagonals.
 * https://oeis.org/A081576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 72, u32 n <= 181, u64 n <= 425,
 *            u128 n <= 1159, big any n
 * Helpers:   binomial_coeff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A081576
   a(n) = value obtained by reading the square array
   T(i,k) = Σ_{j=0}^{k} C(k,j) * Fibonacci(j) * i^{k-j}
   along antidiagonals (i ≥ 0, k ≥ 0).  The array starts with
   T(0,0)=0, T(0,1)=1, T(1,0)=0, T(1,1)=1, etc. */

/* Helpers – use only integer arithmetic on seq_t and idx_t */
#include <stdlib.h>  /* for malloc, free */
#include "../utils/binomial_coeff.h"

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find diagonal d so that S(d-1) <= n < S(d), where
       S(d) = (d+1)(d+2)/2 is the number of terms up to that diagonal. */
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 <= n)  ++d;

    /* Previous total terms before this diagonal. */
    idx_t prev_total = d * (d + 1) / 2;

    /* Offset k along the diagonal: 0 <= k <= d. */
    idx_t k = n - prev_total;

    /* Row index and column index. */
    idx_t i = d - k;   /* row */
    idx_t col = k;     /* column */

    /* Precompute Fibonacci numbers up to k. */
    seq_t *fib_arr = (seq_t*)malloc((k + 1) * sizeof(seq_t));
    if (fib_arr == NULL) return 0;  /* out of memory, return 0 */
    fib_arr[0] = 0;
    if (k >= 1) fib_arr[1] = 1;
    for (idx_t jj = 2; jj <= k; ++jj) {
        fib_arr[jj] = fib_arr[jj - 1] + fib_arr[jj - 2];
    }

    /* Sum over j = 0..k.  Binomial coefficients are generated iteratively. */
    seq_t sum = 0;
    seq_t bin = 1;                     /* C(k,0) */
    for (idx_t j = 0; j <= k; ++j) {
        seq_t term = bin * fib_arr[j] * pow_seq((seq_t)i, k - j);
        sum += term;
        if (j < k) {
            /* Update binomial coefficient to C(k, j+1). */
            bin = bin * (k - j);
            bin = bin / (j + 1);
        }
    }

    free(fib_arr);
    return sum;
}
