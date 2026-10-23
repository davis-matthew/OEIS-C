/*
 * A042681  Denominators of continued fraction convergents to sqrt(870).
 * https://oeis.org/A042681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 118*a(n-2) - a(n-4) for n>3 Initial terms: a(0)=1,
 *            a(1)=2, a(2)=117, a(3)=236
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of computed terms */
    static seq_t *buf = NULL;
    static size_t buf_size = 0;
    static idx_t max_computed = (idx_t)-1;  /* not yet initialized */

    /* Allocate or grow cache if necessary */
    size_t need = (n > 3) ? (size_t)(n + 1) : 4;  /* at least 4 slots for the first 4 terms */
    if (buf == NULL || buf_size < need) {
        seq_t *new_buf = (seq_t*)realloc(buf, sizeof(seq_t) * need);
        if (new_buf == NULL) return (seq_t)0;  /* allocation failure: return 0 */
        buf = new_buf;
        buf_size = need;
        /* Initialize the first four terms */
        buf[0] = (seq_t)1;
        buf[1] = (seq_t)2;
        buf[2] = (seq_t)117;
        buf[3] = (seq_t)236;
        max_computed = 3;
    }

    /* Compute any missing terms up to n */
    if (max_computed < n) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            size_t idx = (size_t)i;
            if (i <= 3) {
                /* These have already been set during initialization */
                continue;
            }
            /* Recurrence: a(n) = 118*a(n-2) - a(n-4) */
            seq_t term = (seq_t)118 * buf[idx - 2] - buf[idx - 4];
            buf[idx] = term;
        }
        max_computed = n;
    }

    return buf[(size_t)n];
}
