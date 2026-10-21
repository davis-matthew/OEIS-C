/*
 * A051405  a(n) = (3^n+1)*(3^(n+1)+1)/4.
 * https://oeis.org/A051405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n+1)*(3^(n+1)+1)/4 Recurrence: a(0)=2, a(1)=10,
 *            a(2)=70; a(n)=13*a(n-1)-39*a(n-2)+27*a(n-3)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A051405 */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *vals = NULL;   /* memoised values */
    static idx_t len = 0;        /* number of stored terms (0 .. len-1) */

    /* Handle small indices directly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)70;

    /* Ensure the buffer is initialised and contains the first three terms */
    if (len < 3) {
        idx_t need = 3;
        vals = (seq_t *)realloc(vals, sizeof(seq_t) * need);
        vals[0] = (seq_t)2;
        vals[1] = (seq_t)10;
        vals[2] = (seq_t)70;
        len   = need;
    }

    /* Return if already computed */
    if (n < len) return vals[n];

    /* Grow the buffer and compute missing terms using the recurrence */
    idx_t newlen = n + 1;
    vals = (seq_t *)realloc(vals, sizeof(seq_t) * newlen);
    for (idx_t i = len; i < newlen; ++i) {
        vals[i] = (seq_t)13 * vals[i-1]
                - (seq_t)39 * vals[i-2]
                + (seq_t)27 * vals[i-3];
    }
    len = newlen;

    return vals[n];
}
