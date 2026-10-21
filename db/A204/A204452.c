/*
 * A204452  A014330 - A203577. Difference between the exponential convolution
 *          of A000108 (Catalan) with itself and the corresponding exponential
 *          half-convolution.
 * https://oeis.org/A204452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=floor(n/2)+1}^{n} binomial(n,k) * C(k) * C(n-k)
 *            where C(m) is the mth Catalan number (A000108)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */

/* Helper: compute binomial(n,k) using multiplicative formula.
   The result is exact; the division by the denominator is always exact. */
static seq_t my_binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i); /* numerator */
        res = res / (seq_t)i;           /* exact division by denominator */
    }
    return res;
}

/* Cache for Catalan numbers. */
static seq_t *C_cache = NULL;   /* array of Catalan numbers */
static idx_t   C_cap   = 0;      /* current allocated size of the cache */

/* Problem-specific function. */
seq_t a(idx_t n)
{
    /* Ensure the Catalan cache contains values up to C(n). */
    if (n + 1 > C_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(C_cache, new_cap * sizeof(seq_t));
        if (!tmp) {                /* allocation failed – return zero */
            return (seq_t)0;
        }
        C_cache = tmp;
        /* If this is the first allocation, set C(0)=1. */
        if (C_cap == 0) {
            C_cache[0] = (seq_t)1;
            C_cap = 1;
        }
        /* Compute missing Catalan numbers using the recurrence: 
           C(0)=1, C(n) = C(n-1) * (4*n-2) / (n+1). */
        for (idx_t i = C_cap; i <= n; ++i) {
            seq_t num = C_cache[i - 1] * (seq_t)(4 * i - 2);
            seq_t den = (seq_t)(i + 1);
            C_cache[i] = num / den;
        }
        C_cap = new_cap;
    }

    idx_t start = n / 2 + 1;   /* floor(n/2)+1 */
    seq_t sum = (seq_t)0;
    for (idx_t k = start; k <= n; ++k) {
        seq_t term = my_binom(n, k);
        term *= C_cache[k];
        term *= C_cache[n - k];
        sum += term;
    }
    return sum;
}
