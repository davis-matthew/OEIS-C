/*
 * A192986  Numerator of Sum_{i=0..n-1} B(i)/B(n), where B(i) = A000110(i) are
 *          the Bell numbers.
 * https://oeis.org/A192986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of (∑_{i=0}^{n-1} A000110(i)) / A000110(n)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Static storage for Bell numbers and their prefix sums. */
static seq_t *Bell      = NULL;
static seq_t *prefixSum = NULL;
static size_t  capacity = 0;   /* allocated slots (>= maxIndex + 1) */
static idx_t   maxIndex = 0;   /* highest index computed so far */

/* Ensure that Bell[0..n] and prefixSum[0..n] are computed. */
static void
ensure(idx_t n)
{
    /* Allocate at least one slot initially. */
    if (capacity == 0) {
        capacity = 1;
        Bell      = (seq_t *)realloc(Bell, capacity * sizeof(seq_t));
        prefixSum = (seq_t *)realloc(prefixSum, capacity * sizeof(seq_t));
        if (!Bell || !prefixSum) return;          /* allocation failed */
        /* B(0) = 1, prefixSum[0] = 1 */
        Bell[0]      = (seq_t)1;
        prefixSum[0] = Bell[0];
        maxIndex     = 0;
    }

    /* Grow the allocated array if needed. */
    if ((size_t)(n + 1) > capacity) {
        size_t newCap = capacity ? capacity * 2 : 1;
        while (newCap <= (size_t)(n + 1)) newCap *= 2;
        Bell      = (seq_t *)realloc(Bell, newCap * sizeof(seq_t));
        prefixSum = (seq_t *)realloc(prefixSum, newCap * sizeof(seq_t));
        if (!Bell || !prefixSum) return;          /* allocation failed */
        capacity = newCap;
    }

    /* Compute missing Bell numbers and prefix sums. */
    for (idx_t i = maxIndex + 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;                     /* C(i-1,0) */
        for (idx_t k = 0; k < i; ++k) {
            sum += Bell[k] * binom;
            if (k + 1 < i)
                binom = binom * (seq_t)(i - 1 - k) / (seq_t)(k + 1);
        }
        Bell[i]      = sum;
        prefixSum[i] = prefixSum[i - 1] + sum;
    }
    maxIndex = n;
}

/* Return the numerator of the reduced fraction
   sum_{i=0}^{n-1} B(i) / B(n). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* empty sum */

    ensure(n);                        /* make Bell[n] available */

    seq_t numerator = prefixSum[n - 1];    /* ∑_{i=0}^{n-1} B(i) */
    seq_t g = gcd(numerator, Bell[n]);     /* reduce the fraction */
    return numerator / g;
}
