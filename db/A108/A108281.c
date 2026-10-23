/*
 * A108281  Numbers that are both triangular and pentagonal of the second
 *          kind.
 * https://oeis.org/A108281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 194*a(n-1) - a(n-2) + 16, a(1)=0, a(2)=15
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *buf = NULL;        /* 1‑based buffer of computed terms */
    static idx_t max_computed = 0;   /* highest index computed so far */

    /* n is assumed ≥1; for n==0 return 0 (undefined but harmless) */
    if (n == 0) return 0;

    if (n > max_computed) {
        /* Allocate or enlarge the buffer */
        idx_t new_max = n;
        seq_t *tmp = (seq_t *)realloc(buf, (new_max + 1) * sizeof(seq_t));
        if (!tmp) abort();           /* abort on allocation failure */
        buf = tmp;

        /* Compute missing terms */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i == 1) {
                buf[1] = (seq_t)0;
            } else if (i == 2) {
                buf[2] = (seq_t)15;
            } else {
                seq_t t = (seq_t)194 * buf[i - 1];
                t = t + (seq_t)16;
                t = t - buf[i - 2];
                buf[i] = t;
            }
        }
        max_computed = n;
    }
    return buf[n];
}
