/*
 * A076902  a(1)=1, a(n) = floor(n/2) - a(floor(n/2)).
 * https://oeis.org/A076902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n ≥ 2, a(n) = floor(n/2) – a(floor(n/2))
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation storage */
    static seq_t *table = NULL;   /* 1‑based array: table[1] = a(1), etc.   */
    static idx_t cap   = 0;       /* capacity of the allocated array       */
    static idx_t max_n = 0;       /* highest n for which a(n) is stored    */

    /* Ensure table exists and is large enough for index n */
    if (n + 1 > cap) {
        idx_t new_cap = cap ? cap : 2;   /* at least 2 entries to hold a(1)      */
        while (n + 1 > new_cap)
            new_cap <<= 1;                 /* double until big enough             */
        seq_t *tmp = (seq_t *)realloc(table, new_cap * sizeof(seq_t));
        table = tmp;
        cap   = new_cap;
    }

    /* Initialise the base case a(1) if needed */
    if (max_n == 0) {
        table[1] = (seq_t)1;
        max_n = 1;
    }

    /* Compute values up to requested n if not already done */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        idx_t half = i >> 1;               /* floor(i/2) */
        table[i] = ((seq_t)half) - table[half];
    }
    max_n = n;

    return table[n];
}
