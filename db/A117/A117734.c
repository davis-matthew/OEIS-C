/*
 * A117734  Absolute difference between the n-th primorial and the n-th
 *          compositorial number.
 * https://oeis.org/A117734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |A034386(n)-A049614(n)|
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 17, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Absolute difference between the product of all primes ≤ n
   and the product of all composite numbers ≤ n.               */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Static state to memoize cumulative products. */
static seq_t *primProd = NULL;      /* product of primes ≤ k */
static seq_t *compProd = NULL;      /* product of composites ≤ k */
static size_t cap = 0;              /* current allocated capacity (elements) */
static idx_t computedUpTo = 0;      /* largest k for which arrays are computed */

/* Ensure the arrays have storage for indices up to n (inclusive). */
static void
ensure_capacity(idx_t n)
{
    if (cap >= (size_t)n + 1)
        return;

    size_t newCap = cap == 0 ? (size_t)n + 1 : cap;
    while (newCap <= (size_t)n)
        newCap <<= 1;

    if (primProd == NULL) {
        primProd = (seq_t *)malloc(newCap * sizeof(seq_t));
        compProd = (seq_t *)malloc(newCap * sizeof(seq_t));
    } else {
        primProd = (seq_t *)realloc(primProd, newCap * sizeof(seq_t));
        compProd = (seq_t *)realloc(compProd, newCap * sizeof(seq_t));
    }

    /* Initialise the first element (index 0) to 1 for both products. */
    if (cap == 0) {
        primProd[0] = 1;
        compProd[0] = 1;
    }
    cap = newCap;
}

/* Compute cumulative products up to index n. */
static void
compute_up_to(idx_t n)
{
    if (n <= computedUpTo)
        return;

    ensure_capacity(n);

    for (idx_t k = computedUpTo + 1; k <= n; ++k) {
        /* Update prime product */
        if (is_prime(k))
            primProd[k] = primProd[k - 1] * (seq_t)k;
        else
            primProd[k] = primProd[k - 1];

        /* Update composite product: skip 1 as it is not composite */
        if (k > 1 && !is_prime(k))
            compProd[k] = compProd[k - 1] * (seq_t)k;
        else
            compProd[k] = compProd[k - 1];
    }
    computedUpTo = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset starts at 1, but guard for 0 */

    compute_up_to(n);

    seq_t p = primProd[n];
    seq_t c = compProd[n];

    return (p >= c) ? (p - c) : (c - p);
}
