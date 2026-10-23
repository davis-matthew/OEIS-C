/*
 * A133218  Indices of triangular numbers (A000217) that are also decagonal
 *          (A001107).
 * https://oeis.org/A133218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A133218 -- Index of triangular numbers that are also decagonal
 *  Recurrence (for n > 5):
 *      a(n) = 34*a(n-2) - a(n-4) + 16
 *  Initial values:
 *      a(1) = 0, a(2) = 1, a(3) = 4, a(4) = 55, a(5) = 154
 *
 *  The function keeps a static cache so that calls in any order are
 *  fast, and it never writes past the allocated buffer.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  Static cache: 1‑based indexing, entry 0 unused. */
    static seq_t *buf  = NULL;  /* array of computed terms */
    static size_t   sz  = 0;     /* allocated size (>= current n+1) */
    static idx_t   high = 0;     /* highest index that has been computed */

    /* Ensure the buffer can address a(n) and allows base initialisation. */
    size_t need = (size_t)n + 1;
    if (need < 6) need = 6;               /* we need space for indices 1..5 */
    if (sz < need) {
        seq_t *tmp = (seq_t *)realloc(buf, need * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure */
        buf = tmp;
        sz  = need;
    }

    /* Initialise the first five values if not already done. */
    if (high < 1) { buf[1] = (seq_t)0;   high = 1; }
    if (high < 2) { buf[2] = (seq_t)1;   high = 2; }
    if (high < 3) { buf[3] = (seq_t)4;   high = 3; }
    if (high < 4) { buf[4] = (seq_t)55;  high = 4; }
    if (high < 5) { buf[5] = (seq_t)154; high = 5; }

    /* Compute missing values up to n using the recurrence. */
    if (high < n) {
        for (idx_t i = high + 1; i <= n; ++i) {
            /* Valid for i ≥ 6; for i = 6..5 we already initialised. */
            buf[i] = (seq_t)34 * buf[i-2] - buf[i-4] + (seq_t)16;
        }
        high = n;
    }

    return buf[n];
}
