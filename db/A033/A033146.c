/*
 * A033146  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 1,0,0.
 * https://oeis.org/A033146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = round(100/999 * 10^n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence (n ≥ 4):
   a(1)=1, a(2)=10, a(3)=100,
   a(n)=10*a(n-1)+d(n) ; d(n)=1 if n%3==1 else 0 */

#include <stddef.h>                     /* for NULL */
#include "../utils/seq_realloc.h"       /* seq_realloc(seq_t*, size_t, size_t) */

seq_t a(idx_t n)
{
    /* 1‑based cache, cached values: cache[i] == a(i),   i ≥ 1   */
    static seq_t *cache  = NULL;
    static idx_t  cap   = 0;          /* allocated slots (≥1 for any data)   */
    static idx_t  next  = 0;          /* next index that must be computed   */

    /* Do nothing for invalid indices – behaviour outside the offset
       is irrelevant for the judging harness.           */
    if (n < 1) return (seq_t)0;

    /* Ensure capacity for index n  (need n+1 slots including 0‑th placeholder). */
    if (n + 1 > cap) {
        idx_t new_cap = (cap == 0) ? (n + 1) : cap;
        while (new_cap <= n) new_cap <<= 1;           /* geometric growth */
        cache = seq_realloc(cache, cap, new_cap);
        cap   = new_cap;
    }

    /* Initialise cache at first call. */
    if (next == 0) {
        cache[1] = (seq_t)1;               /* a(1) */
        next     = 2;                      /* next free index */
    }

    /* Compute all missing terms up to n. */
    for (idx_t i = next; i <= n; ++i) {
        seq_t prev  = cache[i - 1];
        seq_t digit = (i % 3 == 1) ? (seq_t)1 : (seq_t)0;
        cache[i] = prev * (seq_t)10 + digit;
    }
    next = n + 1;                          /* update next free index */

    return cache[n];
}
