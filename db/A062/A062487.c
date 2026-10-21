/*
 * A062487  Nearest integer to (Product(n^((1 + log(1 + i))/i^2), {i, 1, n})).
 * https://oeis.org/A062487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 29, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062487
   Formula: a(n) = NearestInteger[
                    Product[ n^((1 + Log[1 + i])/i^2), {i, 1, n} ]
                  ]
   a(0) = 1 (empty product)
*/

#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

static long double *prefixSum = NULL;  /* prefixSum[1..k] = Σ_{i=1}^k (1+ln(1+i))/i^2 */
static idx_t prefixSize = 0;

/* Ensure that prefixSum[1..n] are computed */
static void ensure_prefix(idx_t n)
{
    if (n <= prefixSize) return;

    idx_t old = prefixSize;
    prefixSize = n;
    prefixSum = (long double *)realloc(prefixSum, (prefixSize + 1) * sizeof(long double));
    if (!prefixSum) return;  /* allocation failure – unlikely */

    long double sum = (old == 0) ? 0.0L : prefixSum[old];
    for (idx_t i = old + 1; i <= n; ++i) {
        long double term = (1.0L + logl((long double)(i + 1))) /
                           ((long double)i * (long double)i);
        sum += term;
        prefixSum[i] = sum;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;  /* empty product */

    ensure_prefix(n);

    /* exponent sum * ln(n) = Σ ((1+ln(1+i))/i^2) * ln(n) */
    long double logProduct = prefixSum[n] * logl((long double)n);
    long double value = expl(logProduct);

    /* nearest integer */
    unsigned long long rounded = (unsigned long long)(value + 0.5L);
    return (seq_t)rounded;
}
