/*
 * A208014  Number of 4 X n 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 10*a(n-3) + 4*a(n-4) - 8*a(n-5) -
 *            16*a(n-6) for n > 8 Initial values: a(1)=9, a(2)=81, a(3)=225,
 *            a(4)=841, a(5)=2304, a(6)=6084, a(7)=18496, a(8)=53824. This
 *            implementation memoises computed terms so that arbitrary n can
 *            be requested in any order; it is fully compatible...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 39, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[0] is unused, cache[1].. hold known terms   */
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* allocated capacity (in seq_t)   */
    static size_t used  = 0;      /* next uninitialised index (1‑based) */

    if (n == 0) return 0;   /* offset 1, below 1 is undefined */

    /* Determine minimum required capacity */
    size_t required = (size_t)n + 1;
    if (required < 9) required = 9;      /* need room for first 8 terms */

    /* Expand cache if necessary */
    if (cap < required) {
        size_t new_cap = cap ? cap << 1 : 16;
        if (new_cap < required) new_cap = required;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                     /* memory exhausted   */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise first 8 terms once */
    if (used == 0) {
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)81;
        cache[3] = (seq_t)225;
        cache[4] = (seq_t)841;
        cache[5] = (seq_t)2304;
        cache[6] = (seq_t)6084;
        cache[7] = (seq_t)18496;
        cache[8] = (seq_t)53824;
        used = 9;            /* next index to compute */
    }

    /* If n is within already computed range, return directly */
    if ((size_t)n < used) return cache[(size_t)n];

    /* Compute terms up to n using the recurrence */
    for (size_t i = used; i <= (size_t)n; ++i) {
        const seq_t a1 = cache[i-1];
        const seq_t a2 = cache[i-2] << 1;      /* *2  */
        const seq_t a3 = cache[i-3] * 10;      /* *10 */
        const seq_t a4 = cache[i-4] << 2;      /* *4  */
        const seq_t a5 = cache[i-5] << 3;      /* *8  */
        const seq_t a6 = cache[i-6] << 4;      /* *16 */

        cache[i] = a1 + a2 + a3 + a4 - a5 - a6;
    }

    used = (size_t)n + 1;
    return cache[(size_t)n];
}
