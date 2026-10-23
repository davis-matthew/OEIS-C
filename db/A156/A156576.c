/*
 * A156576  Square array T(n, k) = Product_{j=1..n} ( Sum_{i=0..j-1}
 *          (i+1)*(k+1)^i ) with T(n, 0) = n!, read by antidiagonals.
 * https://oeis.org/A156576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = read by antidiagonals of the square array T(n,k) defined
 *            by T(n,k) = ∏_{j=1}^{n} (∑_{i=0}^{j-1} (i+1)*(k+1)^i), with
 *            T(n,0) = n!
 * Range:     u8 n <= 18, u16 n <= 25, u32 n <= 41, u64 n <= 73, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Compute factorial of a small non‑negative integer n (n ≤ 10000 for this task). */
static inline seq_t factorial_of(idx_t n)
{
    seq_t f = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        f *= (seq_t)i;
    return f;
}

/* Compute table entry T(n,k). */
static inline seq_t T_of(idx_t n, idx_t k)
{
    if (n == (idx_t)0)
        return (seq_t)1;    /* T(0,k) = 1 for all k */

    if (k == (idx_t)0)
        return factorial_of(n); /* T(n,0) = n! */

    seq_t prod = (seq_t)1;
    seq_t base = (seq_t)(k + (idx_t)1); /* (k+1) */

    for (idx_t j = (idx_t)1; j <= n; ++j) {
        seq_t sum = (seq_t)0;
        seq_t pow = (seq_t)1;             /* (k+1)^i */

        for (idx_t i = (idx_t)0; i < j; ++i) {
            sum += ((seq_t)(i + (idx_t)1)) * pow;
            pow *= base;
        }

        prod *= sum;
    }

    return prod;
}

/* Public sequence routine. */
seq_t a(idx_t n)
{
    /* Find diagonal m such that (m+1)(m+2)/2 > n. */
    idx_t m = (idx_t)0;
    while (((m + (idx_t)1) * (m + (idx_t)2)) / (idx_t)2 <= n)
        ++m;

    /* Index within diagonal (starting from the leftmost element). */
    idx_t offset = n - (m * (m + (idx_t)1)) / (idx_t)2;

    /* Row and column on the antidiagonal. */
    idx_t row = offset;            /* i increases from 0 to m */
    idx_t col = m - offset;        /* k decreases from m to 0 */

    return T_of(row, col);
}
