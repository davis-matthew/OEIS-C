/*
 * A102113  Iccanobirt numbers (3 of 15): a(n) = a(n-1) + R(a(n-2)) +
 *          R(a(n-3)), where R is the digit reversal function A004086.
 * https://oeis.org/A102113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + R(a(n-2)) + R(a(n-3)), with a(0)=0, a(1)=0,
 *            a(2)=1, and R(x) being the digit reversal A004086
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 30, u64 n <= 57, u128 n <= 114,
 *            big any n
 * Helpers:   rev, seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rev.h"
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* An on‑demand memoized linear computation. */
    static seq_t *buf = NULL;   /* cache of computed terms */
    static idx_t  size = 0;     /* number of terms currently computed */
    static idx_t  cap  = 0;     /* allocated capacity of buf */

    /* Initialise the cache on the first call. */
    if (buf == NULL) {
        cap = 3;                  /* we need at least indices 0,1,2 */
        buf = seq_malloc(cap);    /* allocate array of seq_t */
        buf[0] = 0;
        buf[1] = 0;
        buf[2] = 1;
        size = 3;
    }

    /* Grow the buffer if n is beyond the current capacity. */
    if ((idx_t)n >= cap) {
        idx_t new_cap = cap * 2;
        if (new_cap < (idx_t)n + 1)
            new_cap = (idx_t)n + 1;
        buf = seq_realloc(buf, new_cap);
        cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = size; i <= (idx_t)n; ++i) {
        if (i < 3) {
            /* first three terms are already set */
            continue;
        }
        buf[i] = buf[i-1]
               + rev(buf[i-2])
               + rev(buf[i-3]);
    }
    size = (idx_t)n + 1;   /* all indices up to n are now valid */

    return buf[n];
}
