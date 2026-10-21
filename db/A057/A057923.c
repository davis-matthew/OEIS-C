/*
 * A057923  a(0)=0, a(1)=2, a(n) = smallest number such that sequence b(n) =
 *          {a(n-1) BITWISE OR a(n)} is strictly monotonically increasing.
 * https://oeis.org/A057923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 1706, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=0, a(1)=2; for n≥2, a(n)=((a(n-2)|a(n-1))+1)&~a(n-1) */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *a_vals = NULL;   /* array of computed a(n) values */
static size_t a_cap   = 0;     /* current capacity */
static size_t a_len   = 0;     /* number of values stored (== a_cap when full) */

/* Ensure the array has space for at least `needed` elements.  */
static void ensure_cap(size_t needed)
{
    if (needed <= a_cap)
        return;
    size_t new_cap = a_cap ? a_cap * 2 : 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    a_vals = tmp;
    a_cap = new_cap;
    /* a_len remains unchanged – the newly allocated slots are zero‑initialised on demand */
}

/* Compute a(n).  The function is memoised, so arbitrary ordering of calls is supported. */
seq_t a(idx_t n)
{
    /* initialise the table with a(0)=0 and a(1)=2 */
    if (a_vals == NULL) {
        ensure_cap(2);
        a_vals[0] = 0;
        a_vals[1] = 2;
        a_len = 2;
    }

    /* if n already computed, just return it */
    if ((size_t)n < a_len)
        return a_vals[n];

    /* compute subsequent terms up to n */
    for (idx_t i = (idx_t)a_len; i <= n; ++i) {
        /* prev_b = a[i-2] | a[i-1]  */
        seq_t prev_b = a_vals[i-2] | a_vals[i-1];
        seq_t t = prev_b + 1;                               /* t = prev_b + 1   */
        seq_t cand = t & ~a_vals[i-1]; /* minimal a meeting (a[i-1]|a) > prev_b  */
        ensure_cap((size_t)(i + 1));
        a_vals[i] = cand;
    }
    a_len = (size_t)(n + 1);
    return a_vals[n];
}
