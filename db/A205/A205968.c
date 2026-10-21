/*
 * A205968  a(n) = Fibonacci(n)*A008655(n) for n >= 1, with a(0)=1, where
 *          A008655 lists the coefficients in (theta_3(x)*theta_3(3*x) +
 *          theta_2(x)*theta_2(3*x))^4.
 * https://oeis.org/A205968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n) * (24*sigma_3(n) + (n%3==0 ?
 *            216*sigma_3(n/3) : 0)), a(0)=1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 21, u64 n <= 61, u128 n <= 148,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/fib.h"

/* sigma_3(k) = sum of cubes of divisors of k */
static seq_t *sigma3_arr = NULL;
static idx_t sigma3_max  = 0;

static void ensure_sigma3(idx_t n)
{
    if (n <= sigma3_max)
        return;

    /* Resize the array */
    size_t new_size = (size_t)n + 1;
    seq_t *new_arr = (seq_t *)realloc(sigma3_arr, new_size * sizeof(seq_t));
    if (!new_arr) exit(1);
    sigma3_arr = new_arr;

    /* Initialize new entries to zero */
    for (idx_t i = sigma3_max + 1; i <= n; ++i)
        sigma3_arr[i] = 0;

    /* Add contributions from divisors that were already known
       to indices larger than the previous maximum and ≤ n */
    for (idx_t i = 1; i <= sigma3_max; ++i) {
        seq_t term = (seq_t)i * i * i;
        idx_t start = (sigma3_max / i + 1) * i;
        for (idx_t j = start; j <= n; j += i)
            sigma3_arr[j] += term;
    }

    /* Add contributions from new divisors i ∈ [sigma3_max+1, n] */
    for (idx_t i = sigma3_max + 1; i <= n; ++i) {
        seq_t term = (seq_t)i * i * i;
        for (idx_t j = i; j <= n; j += i)
            sigma3_arr[j] += term;
    }

    sigma3_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    ensure_sigma3(n);

    /* Compute A008655(n) = 24*σ₃(n) + (n%3==0 ? 216*σ₃(n/3) : 0) */
    seq_t sigma3_n = sigma3_arr[n];
    seq_t A = (seq_t)24 * sigma3_n;
    if (n % 3 == 0) {
        idx_t m = n / 3;
        seq_t sigma3_m = sigma3_arr[m];
        A += (seq_t)216 * sigma3_m;
    }

    /* Fibonacci(n) * A */
    seq_t fn = fib(n);
    return fn * A;
}
