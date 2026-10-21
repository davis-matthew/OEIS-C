/*
 * A066767  a(n) = Sum_{k=1..n} sigma(k)*2^(n-k) where sigma(k) = A000203(k)
 *          is the sum of divisors of k.
 * https://oeis.org/A066767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} sigma(k) * 2^(n-k) Recurrence: a(1) =
 *            sigma(1) = 1 a(n) = 2 * a(n-1) + sigma(n) (n >= 2)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Predicate: seq_t and idx_t types are supplied by the build. */

static seq_t *sigma_arr = NULL;   /* sigma(k) for k <= current_max */
static seq_t *a_arr     = NULL;   /* a(k) for k <= current_max */
static idx_t  current_max = 0;

/* Recompute sigma and a values up to new_max.  For small n this is
   inexpensive (complexity O(n log n)). */
static void recompute(idx_t new_max)
{
    /* Allocate arrays of size new_max+1 (index 0 unused) */
    sigma_arr = (seq_t *)realloc(sigma_arr, (size_t)(new_max + 1) * sizeof(seq_t));
    a_arr     = (seq_t *)realloc(a_arr,     (size_t)(new_max + 1) * sizeof(seq_t));

    /* Initialize sigma to 0 */
    for (idx_t i = 1; i <= new_max; ++i) {
        sigma_arr[i] = 0;
    }

    /* Compute sigma via the divisor summation sieve */
    for (idx_t d = 1; d <= new_max; ++d) {
        for (idx_t j = d; j <= new_max; j += d) {
            sigma_arr[j] += (seq_t)d;
        }
    }

    /* Compute a using the recurrence */
    a_arr[1] = sigma_arr[1];           /* a(1) = 1 */
    for (idx_t i = 2; i <= new_max; ++i) {
        a_arr[i] = (seq_t)2 * a_arr[i-1] + sigma_arr[i];
    }

    current_max = new_max;
}

seq_t a(idx_t n)
{
    /* Ensure data for indices 1..n exists */
    if (n == 0)
        return (seq_t)0; /* undefined for n<1, but return 0 */

    if (n > current_max) {
        recompute(n);
    }

    return a_arr[n];
}
