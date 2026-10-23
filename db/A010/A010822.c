/*
 * A010822  Expansion of Product_{k>=1} (1 - x^k)^15.
 * https://oeis.org/A010822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 7, i32 n <= 399, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A010822 : Expansion of Product_{k>=1} (1 - x^k)^15
   Recurrence: a(0) = 1
   a(n) = -(15 / n) * Σ_{k=1..n} A000203(k) * a(n-k)   (n > 0)
   where A000203(k) is the sum of divisors function σ(k).
   This implementation caches σ and a in static arrays and grows them
   as needed.  It works for any seq_t/idx_t types provided by the build.
*/

#include <stdlib.h>

/* Static caches for divisor sums (σ) and the sequence values (a). */
static seq_t *sigma_arr = NULL;   /* sigma_arr[i] = σ(i) */
static size_t sigma_cap = 0;      /* current capacity of sigma_arr */

static seq_t *a_arr = NULL;       /* a_arr[i]   = a(i)   */
static size_t a_cap = 0;          /* current capacity of a_arr */

/* Recompute σ(1..n) and store in sigma_arr.  For simplicity we recompute
   the whole array when n increases.  Complexity O(n log n). */
static void ensure_sigma(idx_t n)
{
    if ((size_t)n < sigma_cap) return;          /* already enough */

    size_t new_cap = (size_t)n + 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) abort();                      /* out of memory */

    /* initialise to zero */
    for (size_t i = 0; i < new_cap; ++i) {
        new_arr[i] = 0;
    }

    /* compute σ by harmonic sieve */
    for (size_t i = 1; i <= (size_t)n; ++i) {
        for (size_t j = i; j <= (size_t)n; j += i) {
            new_arr[j] += (seq_t)i;
        }
    }

    /* replace old array */
    free(sigma_arr);
    sigma_arr = new_arr;
    sigma_cap = new_cap;
}

/* Compute a(0..n) incrementally, using the already available σ
   values.  For each new i we perform the convolution sum.  Complexity
   O(n^2). */
static void ensure_a(idx_t n)
{
    if ((size_t)n < a_cap) return;              /* already computed */

    size_t new_cap = (size_t)n + 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) abort();                      /* out of memory */

    /* copy already known values */
    for (size_t i = 0; i < a_cap; ++i) {
        new_arr[i] = a_arr[i];
    }

    /* compute the new values */
    for (size_t i = a_cap; i < new_cap; ++i) {
        if (i == 0) {                          /* base case */
            new_arr[0] = 1;
            continue;
        }
        seq_t sum = 0;
        for (size_t k = 1; k <= i; ++k) {
            sum += sigma_arr[k] * new_arr[i - k];
        }
        seq_t temp = (seq_t)15 * sum;
        new_arr[i] = -temp / (seq_t)i;
    }

    /* replace old array */
    free(a_arr);
    a_arr = new_arr;
    a_cap = new_cap;
}

/* Public function required by the interface.  It grows the caches as
   necessary and returns a(n). */
seq_t a(idx_t n)
{
    if (n < 0) {
        /* Undefined for negative indices; return 0. */
        return (seq_t)0;
    }
    ensure_sigma(n);
    ensure_a(n);
    return a_arr[(size_t)n];
}
