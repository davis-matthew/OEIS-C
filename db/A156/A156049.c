/*
 * A156049  Triangle read by rows: T(n, k) = binomial(n, k) + 2*(1 + n! - k! -
 *          (n-k)!).
 * https://oeis.org/A156049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,k) where i = largest integer with i(i+1)/2 <= n, i.e.
 *            the row containing n k = n - i(i+1)/2 T(i,k) = binomial(i,k) +
 *            2*(1 + i! - k! - (i-k)!) (triangle read by rows, offset 0)
 * Range:     u8 n <= 21, u16 n <= 36, u32 n <= 91, u64 n <= 231,
 *            u128 n <= 595, big any n
 * Validated: standard, pass, 71 terms through n = 4812 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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
    /* Find the row i that contains the nth element (0‑based). */
    idx_t i = 0;
    idx_t start;
    for (;;)
    {
        start = i * (i + 1) / 2;
        if (start > n)   /* the first start greater than n -> row is i-1 */
            break;
        ++i;
    }
    --i;                      /* current row index */
    start = i * (i + 1) / 2;   /* first index in this row */
    idx_t k = n - start;      /* column index within the row */

    /* Compute the triangle entry T(i,k). */
    seq_t val = binomial(i, k);

    seq_t tmp = factorial(i);
    tmp -= factorial(k);
    tmp -= factorial(i - k);
    tmp += 1;
    tmp *= 2;

    val += tmp;
    return val;
}
