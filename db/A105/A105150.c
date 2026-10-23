/*
 * A105150  Approximation to leading digit of n-th Fibonacci number.
 * https://oeis.org/A105150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Approximation to leading digit of n‑th Fibonacci number
 * Sequence definition (OEIS A105150):
 *   a(0) = 0, a(1) = a(2) = 1
 *   For n > 2:
 *     let x = a(n-3), y = a(n-2), z = a(n-1)
 *     if (z == 1)      w = x + 2*y
 *     else             w = y + z          // z ≥ 2
 *     a(n) = leading digit of w, i.e.
 *            a(n) = (w < 10) ? w : w/10
 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache of computed terms, stored as 8‑bit values */
    static uint8_t *c = NULL;
    static idx_t  last = 0;

    /* allocate and seed initial terms if needed */
    if (c == NULL) {
        /* ensure capacity for at least 3 terms */
        c = (uint8_t *)malloc(3 * sizeof(uint8_t));
        if (c == NULL) return 0;                  /* out of memory */
        c[0] = 0;  /* a(0) */
        c[1] = 1;  /* a(1) */
        c[2] = 1;  /* a(2) */
        last  = 2;
    }

    /* compute any missing terms up to n */
    if (n > last) {
        /* grow the buffer to hold up to n */
        uint8_t *tmp = (uint8_t *)realloc(c, (size_t)(n+1) * sizeof(uint8_t));
        if (tmp == NULL) return 0;                 /* out of memory */
        c = tmp;

        /* fill remaining entries */
        for (idx_t i = last + 1; i <= n; ++i) {
            uint8_t x = c[i-3];
            uint8_t y = c[i-2];
            uint8_t z = c[i-1];
            uint8_t w;
            if (z == 1)
                w = x + 2 * y;
            else
                w = y + z;          /* z >= 2 */

            /* leading digit of w */
            c[i] = (w < 10) ? w : w / 10;
        }
        last = n;
    }

    return (seq_t)c[n];
}
