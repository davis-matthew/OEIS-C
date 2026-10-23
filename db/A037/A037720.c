/*
 * A037720  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,1,3.
 * https://oeis.org/A037720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 8*a(n-1) + digit_n, with digit_n = {2,0,1,3}[(n-1) mod
 *            4], a(1)=2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* external types supplied by the build system */
extern seq_t a(idx_t n);

/* memoised calculation of a(n) */
seq_t a(idx_t n)
{
    /* the sequence is defined only for n >= 1; guard against n==0 */
    if (n == 0)
        return 0;

    /* static cache holding previously computed values.
       cache[0] is unused, but we initialise it to 0 to ease the recurrence. */
    static seq_t *cache     = NULL;
    static size_t cached    = 0;   /* highest index stored (0 means empty) */

    /* grow cache if needed */
    if ((size_t)n > cached) {
        size_t new_size = (size_t)n + 1;      /* we need indices 0 .. n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();              /* allocation failed */

        cache = new_cache;

        /* initialise index 0 which is required by the recurrence */
        cache[0] = 0;

        /* digits of the base‑8 expansion: 2,0,1,3 repeating */
        static const seq_t digits[4] = {2, 0, 1, 3};

        /* compute values from cached+1 up to n */
        for (idx_t i = cached + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] * (seq_t)8 + digits[(i - 1) & 3];
        }
        cached = n;
    }

    return cache[n];
}
