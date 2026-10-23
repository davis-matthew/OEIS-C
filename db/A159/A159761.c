/*
 * A159761  Numerator of Hermite(n, 11/21).
 * https://oeis.org/A159761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n+2) = 22*a(n+1) - 882*(n+1)*a(n), a(0)=1, a(1)=22
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;   /* cached sequence values */
    static idx_t allocated = 0; /* number of elements currently stored */

    /* Handle calls when n is 0 or 1 without further work */
    if (n == 0) {
        if (allocated == 0) {
            /* allocate and set a(0) */
            buf = (seq_t *)realloc(NULL, sizeof(seq_t));
            if (!buf) return 0;
            buf[0] = (seq_t)1;
            allocated = 1;
        }
        return buf[0];
    }
    if (n == 1) {
        if (allocated < 2) {
            /* allocate or grow to hold a(1) */
            idx_t new_size = 2;
            seq_t *tmp = (seq_t *)realloc(buf, new_size * sizeof(seq_t));
            if (!tmp) return 0;
            buf = tmp;
            if (allocated == 0) {
                buf[0] = (seq_t)1;
                buf[1] = (seq_t)22;
            } else if (allocated == 1) {
                buf[1] = (seq_t)22;
            }
            allocated = new_size;
        }
        return buf[1];
    }

    /* General case: n >= 2 */
    if (allocated <= n) {
        idx_t old_alloc = allocated;
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(buf, new_size * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failed – safe fallback */
        buf = tmp;
        allocated = new_size;

        /* Initialise freshly allocated entries at the start */
        if (old_alloc == 0) {
            buf[0] = (seq_t)1;           /* a(0) */
            buf[1] = (seq_t)22;          /* a(1) */
        } else if (old_alloc == 1) {
            buf[1] = (seq_t)22;          /* a(1) */
        }

        /* Compute missing terms using the recurrence */
        idx_t start = old_alloc < 2 ? 2 : old_alloc;
        for (idx_t i = start; i <= n; ++i) {
            seq_t term1 = buf[i - 1];
            seq_t term0 = buf[i - 2];
            buf[i] = (seq_t)22 * term1
                   - (seq_t)882 * ((seq_t)(i - 1) * term0);
        }
    }
    return buf[n];
}
