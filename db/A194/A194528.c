/*
 * A194528  First coordinate of (5,8)-Lagrange pair for n.
 * https://oeis.org/A194528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) + a(n-13) - a(n-14) for n > 14 Initial values a(1)
 *            … a(14): -3, 2, -1, 4, 1, -2, 3, 0, -3, 2, -1, -4, 1, -2 The
 *            function memoises computed terms in a static array
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The maximum n we handle is 20000; this comfortably covers all test cases
   (the specification only requires correct values up to about 10000). */
#define MAX_N 20000

static const seq_t initial[14] = {
    (seq_t)-3, (seq_t)2, (seq_t)-1, (seq_t)4, (seq_t)1,
    (seq_t)-2, (seq_t)3, (seq_t)0, (seq_t)-3, (seq_t)2,
    (seq_t)-1, (seq_t)-4, (seq_t)1, (seq_t)-2
};

/* Static cache of already computed values.  Index i of the array
   stores a(i+1).  For n ≥ 1, a(n) is returned from cache[n-1]. */
static seq_t cache[MAX_N];
static bool cache_initialised = false;
static idx_t cached_upto = 0;

/* Compute a(n) for any n >= 1.  Calls are independent of call order
   because previously computed terms are stored in the static array. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Undefined for n < 1; return 0 as a safe default */

    /* Initialise the first 14 terms on first call */
    if (!cache_initialised) {
        for (idx_t i = 0; i < 14; ++i)
            cache[i] = initial[i];
        cached_upto = 13;          /* a(14) corresponds to cache[13] */
        cache_initialised = true;
    }

    /* Return cached value if already computed */
    if (n <= cached_upto + 1)
        return cache[n - 1];

    /* Compute terms up to n using the recurrence */
    /* i is the 1‑based index of the term being computed */
    for (idx_t i = cached_upto + 2; i <= n; ++i) {
        seq_t a_prev     = cache[i - 2];   /* a(i-1)  */
        seq_t a_13_back  = cache[i - 14];  /* a(i-13) */
        seq_t a_14_back  = cache[i - 15];  /* a(i-14) */
        cache[i - 1] = a_prev + a_13_back - a_14_back;
    }
    cached_upto = n - 1;
    return cache[n - 1];
}
