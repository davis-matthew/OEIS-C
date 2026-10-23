/*
 * A145607  Numbers k such that (3*(2*k + 1)^2 + 2)/5 is a square.
 * https://oeis.org/A145607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+2) = 8*a(n+1) – a(n) + 3 with a(1) = 0, a(2) = 4. Compute in
 *            O(n) using a small static cache
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t cache[20001];        /* cache[i] holds a(i+1)  , up to i = 20000-1 */
static idx_t stored = 0;          /* number of terms currently cached */

/* initialise the first two terms once */
static void
init_initial_terms(void)
{
    if (stored > 0)
        return;
    cache[0] = (seq_t)0;          /* a(1) */
    cache[1] = (seq_t)4;          /* a(2) */
    stored   = 2;
}

/* compute a(n) for n >= 1 */
seq_t
a(idx_t n)
{
    if (n == 0)                    /* undefined for n = 0, return 0 as a safe placeholder */
        return (seq_t)0;

    init_initial_terms();

    if (n <= stored)
        return cache[n - 1];

    /* grow cache up to n */
    if (n > 20000) {               /* guard against exceeding static size */
        abort();                   /* or return some sentinel value if preferred */
    }

    for (idx_t i = stored; i < n; ++i) {
        /* recurrence: a(i+1) = 8*a(i) – a(i-1) + 3  (i starts at 1 for a(3)) */
        cache[i] = ( (seq_t)8 * cache[i - 1] ) - cache[i - 2] + (seq_t)3;
    }
    stored = n;
    return cache[n - 1];
}
