/*
 * A174690  Triangle T(n, k) = n!*binomial(n, k) - n! + 1, read by rows.
 * https://oeis.org/A174690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(m,k) = m!*C(m,k) – m! + 1, where n is the (n+1)-th
 *            entry of the triangle read row‑wise; m is the row index and k
 *            the column index
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 68, u64 n <= 174,
 *            u128 n <= 446, big any n
 * Validated: standard, pass, 71 terms through n = 4886 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    /* Determine row m and column k for linear index n. */
    idx_t m      = 0;
    idx_t prefix = 0;            /* cumulative count of previous rows */

    /* Incrementally count rows until we surpass n. */
    while (prefix + m + 1 <= n) {
        prefix += m + 1;
        ++m;
    }

    idx_t k = n - prefix;        /* column within row m */

    /* Compute m! and binomial(m,k). */
    seq_t fact = factorial(m);
    seq_t bin  = binomial(m, k);

    /* Apply the defining formula. */
    seq_t result = fact * bin;   /* m! * C(m,k) */
    result -= fact;              /* subtract m! */
    result += 1;                 /* add 1 */

    return result;
}
