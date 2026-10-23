/*
 * A042737  Denominators of continued fraction convergents to sqrt(898).
 * https://oeis.org/A042737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 1, a(1) = 1; a(n) = p(n) * a(n-1) + a(n-2) where p(n) is
 *            periodic [1, 28, 1, 58] starting at n = 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute denominators of convergents to sqrt(898). */

#include <stddef.h>
#include <stdlib.h>

/* Periodic partial quotients for sqrt(898). */
static const seq_t period[4] = { (seq_t)1, (seq_t)28, (seq_t)1, (seq_t)58 };

/* Memoized storage of already computed terms. */
static seq_t *den = NULL;    /* array of denominators */
static size_t cap = 0;       /* current capacity of the array */
static idx_t last = 0;       /* largest n for which den[n] is valid */

/* Grow the buffer to hold at least 'needed' entries. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cap)
        return;

    size_t new_cap = cap ? cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(den, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* allocation failure – unlikely in test harness */

    den = tmp;
    cap = new_cap;

    /* Initialise first two terms if not already done. */
    if (last < 1) {
        den[0] = (seq_t)1;
        if (cap >= 2)
            den[1] = (seq_t)1;
        last = 1;
    }
}

seq_t a(idx_t n)
{
    /* Initialise memoisation on first invocation. */
    if (!den)
        ensure_capacity((size_t)n + 1);

    /* Base cases. */
    if (n == 0) return den[0];
    if (n == 1) return den[1];

    /* Compute any missing values up to n (order of calls does not matter). */
    ensure_capacity((size_t)n + 1);
    for (idx_t i = last + 1; i <= n; ++i) {
        /* Periodic coefficient: period[(i-1) % 4] */
        size_t idx = (size_t)((i - 1) % 4);
        den[i] = period[idx] * den[i - 1] + den[i - 2];
    }
    last = n;
    return den[n];
}
