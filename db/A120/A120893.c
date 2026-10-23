/*
 * A120893  a(n) = 3*a(n-1) + 3*a(n-2) - a(n-3); a(0)=1, a(1)=1, a(2)=5.
 * https://oeis.org/A120893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - a(n-3) a(0) = 1, a(1) = 1, a(2) = 5
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cached = NULL;      /* dynamically allocated cache */
static idx_t  max_cached = 0;     /* highest index for which cached[n] is valid */

/* Ensure that cached[0 .. n] exist. Grow array if necessary. */
static void ensure_cached(idx_t n)
{
    if (n < max_cached)
        return;

    /* allocate at least up to n+1 */
    size_t new_cap = (size_t)(n + 1);
    cached = (seq_t *)realloc(cached, new_cap * sizeof(seq_t));
    if (!cached) abort();           /* allocation failure */

    /* initialise new entries */
    if (max_cached == 0) {
        cached[0] = (seq_t)1;
        if (new_cap > 1) cached[1] = (seq_t)1;
        if (new_cap > 2) cached[2] = (seq_t)5;
    } else if (max_cached == 1) {
        cached[1] = (seq_t)1;
        if (new_cap > 2) cached[2] = (seq_t)5;
    } else if (max_cached == 2) {
        cached[2] = (seq_t)5;
    }

    /* compute any intermediate terms */
    for (idx_t i = max_cached; i <= n; ++i) {
        if (i < 3) continue;        /* already set */
        cached[i] = (seq_t)3 * cached[i-1]
                  + (seq_t)3 * cached[i-2]
                  - cached[i-3];
    }

    max_cached = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cached(n);
    return cached[n];
}
