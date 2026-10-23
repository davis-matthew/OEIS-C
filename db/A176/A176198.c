/*
 * A176198  Triangle, read by rows, T(n, k) = f(n,k,q) - f(n,0,q) + 1, where
 *          f(n, k, q) = [x^k](p(x,n,q)), p(x, n, q) = (1-x)^(n+1)*Sum_{k >=
 *          0} ( (q*k+1)^n + (q*(k+1)-1)^n )*x^k, and q = 2.
 * https://oeis.org/A176198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Uses the triangle definition from OEIS A176198: For q = 2, the
 *            triangle entries T(n,k) are given by T(n,k) = f(n,k) - f(n,0) +
 *            1, where f(n,k) = [x^k] (1−x)^(n+1) * Σ_{t≥0} ( (2t+1)^n +
 *            (2(t+1)-1)^n ) x^t = 2 * Σ_{i=0}^{k} (-1)^i C(n+1,i)
 *            (2(k-i)+1)^n. Since f(n,0) = 2, we have T(n,k) = f(n,k) –...
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 69, u64 n <= 160,
 *            u128 n <= 446, big any n
 * Helpers:   binom, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/binom.h"   /* seq_t binom(idx_t a, idx_t b); */
#include "../utils/pow_uint.h"/* seq_t pow_uint(uint64_t base, idx_t exp); */

static seq_t compute_f(idx_t row, idx_t k)
{
    seq_t sum_even = 0;
    seq_t sum_odd  = 0;
    for (idx_t i = 0; i <= k; ++i) {
        seq_t binom_coeff = binom(row + 1, i);
        uint64_t base     = 2 * (k - i) + 1;          /* odd positive integer */
        seq_t pow_val     = pow_uint(base, row);      /* (2(k-i)+1)^row */
        seq_t term        = binom_coeff * pow_val;    /* binom * power */

        if (i & 1U) /* odd i */
            sum_odd += term;
        else         /* even i */
            sum_even += term;
    }
    seq_t diff = sum_even - sum_odd;   /* sum_even ≥ sum_odd for all (row,k) */
    return diff * 2;                  /* f(row,k) = 2*(sum_even - sum_odd) */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1; /* first element of the triangle */

    /* Determine row r such that (r+1)(r+2)/2 > n ≥ r(r+1)/2 */
    long double rad = sqrt((long double)(8.0L * n + 1.0L));
    idx_t r2 = (idx_t)((rad - 1.0L) / 2.0L) + 1; /* r2 = r + 1 */
    idx_t r = r2 - 1;

    /* Linear index of first element of this row */
    idx_t prev = r * (r + 1) / 2;
    idx_t k = n - prev;              /* column within the row */

    /* Compute the triangle entry */
    seq_t f_val = compute_f(r, k);
    return f_val - 1;                 /* T(n,k) = f – 1 */
}
