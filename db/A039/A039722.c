/*
 * A039722  a(1) = 1, a(m+1) = 2*Sum_{k=1..floor((m+1)/2)} a(k).
 * https://oeis.org/A039722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Recurrence a(1) = 1 a(m+1) = 2 * sum_{k=1}^{floor((m+1)/2)} a(k)
 *            for m >= 1 Equivalent: a(i) = 2 * pref[floor(i/2)], where
 *            pref[x] = sum_{k=1}^x a(k) The implementation uses memoization
 *            and prefix sums for O(n) time
 * Range:     u8 n <= 21, u16 n <= 89, u32 n <= 609, u64 n <= 8307,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *term = NULL; /* stores a(i) with 1-based index */
static seq_t *pref  = NULL; /* stores prefix sums pref(i) = sum_{k=1}^i a(k) */
static size_t cap   = 0;    /* current allocated capacity (>= n+1) */
static size_t len   = 0;    /* highest index computed so far */

static void ensure_capacity(size_t required)
{
    if (cap >= required + 1)
        return;

    size_t new_cap = cap ? cap * 2 : 16;
    while (new_cap < required + 1)
        new_cap *= 2;

    term = (seq_t *)realloc(term, new_cap * sizeof(seq_t));
    pref = (seq_t *)realloc(pref, new_cap * sizeof(seq_t));
    /* If realloc fails, the original pointer remains, but per the
       constraints we assume sufficient memory is available. */
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* Undefined for n<1, but safe guard */

    ensure_capacity((size_t)n);

    /* Compute terms up to n if not already done */
    for (size_t i = len + 1; i <= (size_t)n; ++i)
    {
        if (i == 1) {
            term[1] = (seq_t)1;
            pref[1]  = term[1];
        } else {
            size_t half = i / 2;                  /* floor(i/2) */
            term[i] = (seq_t)2 * pref[half];     /* a(i) = 2 * pref[half] */
            pref[i]  = pref[i - 1] + term[i];     /* prefix sum */
        }
    }

    len = (size_t)n; /* Update highest computed index */

    return term[(size_t)n];
}
