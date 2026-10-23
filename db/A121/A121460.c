/*
 * A121460  Triangle read by rows: T(n,k) is the number of nondecreasing Dyck
 *          paths of semilength n, having k returns to the x-axis (1<=k<=n).
 * https://oeis.org/A121460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   binomial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A121460: Triangle read by rows, T(n,k) = # of nondecreasing Dyck paths
 * The function a(n) returns the n‑th term of the triangle read row‑by‑row.
 * Formula (from OEIS):
 *   T(n,1)          = fib(2*n-3)   (n >= 2),   T(1,1)=1
 *   T(n,k) (k>=2)   = binom(n-2,k-2) + sum_{j=1}^{n-k} fib(2*j-1)*binom(n-2-j,k-2)
 *
 * The code uses helper functions fib(idx_t) and binomial(idx_t, idx_t) from the
 * utilities package. */
#include <stdint.h>
#include "../utils/fib.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    idx_t cumulative = 1;                /* sum of first r rows */
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }
    idx_t prev_sum = cumulative - r;     /* sum of rows before r */
    idx_t k = n - prev_sum;              /* column index (1‑based) */

    if (r == 1) {
        return (seq_t)1;
    }

    if (k == 1) {
        /* T(r,1) = fib(2*r-3) */
        return fib(2*r - 3);
    }

    /* For k >= 2 */
    seq_t result = binomial(r - 2, k - 2);

    for (idx_t j = 1; j <= r - k; ++j) {
        seq_t f = fib(2*j - 1);
        seq_t b = binomial(r - 2 - j, k - 2);
        result += f * b;
    }

    return result;
}
