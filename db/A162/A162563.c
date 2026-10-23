/*
 * A162563  a(n) = ((5+sqrt(3))*(2+sqrt(3))^n + (5-sqrt(3))*(2-sqrt(3))^n)/2.
 * https://oeis.org/A162563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 4*a(n-1) - a(n-2),  a(0) = 5,  a(1) = 13
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *vals = NULL;      /* cache of computed terms   */
    static idx_t  last = 0;         /* highest index computed    */

    /* Handle the first two terms without allocation */
    if (n == 0) return 5;
    if (n == 1) return 13;

    /* Allocate or grow the cache if necessary */
    if (vals == NULL || last < n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(vals, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed – aborting (should not happen in tests) */
            abort();
        }
        vals = tmp;

        /* Initialize from scratch if this is the first allocation */
        if (last == 0) {
            vals[0] = 5;
            vals[1] = 13;
            last  = 1;
        }

        /* Fill missing entries using the recurrence */
        for (idx_t i = last + 1; i <= n; ++i) {
            vals[i] = vals[i-1] * 4 - vals[i-2];
        }
        last = n;
    }

    return vals[n];
}
