/*
 * A117377  Riordan array (1-4x,x(1-x)).
 * https://oeis.org/A117377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = Flattened row/col of T(r,k) with
 *            T(r,k)=(-1)^(r-k)*(C(k,r-k)+4*C(k,r-k-1)) where C is the
 *            binomial coefficient
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Compute binomial coefficient for small n (n <= 2000 is safe in all cases). */
static seq_t binomial_small(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k == 0 || k == n)
        return 1;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* C(n,k) = C(n,k-1)*(n-k+1)/k – division is exact. */
        res = (res * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return res;
}

/* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2.
   Uses a double approximation followed by correction. */
static idx_t find_row(idx_t n)
{
    double approx = sqrt((double)(8.0 * n + 1.0));
    idx_t r = (idx_t)((approx - 1.0) / 2.0);
    /* Correct possible off‑by‑one errors. */
    while ((unsigned __int128)r * (r + 1) / 2 > n)
        --r;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* Determine row r and column k from flattened index n. */
    idx_t r = find_row(n);
    idx_t prev_rows = r * (r + 1) / 2;       /* triangular number */
    idx_t k = n - prev_rows;                 /* 0 <= k <= r */
    idx_t d = r - k;                         /* r-k = d */

    /* Compute the two binomial terms, guarding against invalid arguments. */
    seq_t term1 = (d <= k) ? binomial_small(k, d) : 0;
    seq_t term2 = (d >= 1) ? binomial_small(k, d - 1) : 0;

    seq_t sum  = term1 + (seq_t)4 * term2;
    seq_t sign = (d & (idx_t)1u) ? -1 : 1;
    return sign * sum;
}
