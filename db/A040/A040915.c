/*
 * A040915  Continued fraction for sqrt(946).
 * https://oeis.org/A040915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction for sqrt(946) computed by standard algorithm
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Constants for sqrt(946) */
    const uint32_t D = 946;
    const uint32_t A0 = 30;          /* floor(sqrt(946)) */
    const uint32_t A_TERMINATOR = 2 * A0;  /* 60, end of period */

    /* Static storage for the periodic part (a(1) .. a(period_len)) */
    static seq_t *period = NULL;
    static size_t period_len = 0;

    /* Build the period once */
    if (!period) {
        size_t cap = 32;           /* initial capacity */
        period = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!period) return (seq_t)0;   /* safety on allocation failure */

        uint32_t m = 0, d = 1, a = A0;
        size_t len = 0;

        while (1) {
            /* Next terms of the continued fraction algorithm */
            m = d * a - m;
            d = (D - m * m) / d;
            a = (A0 + m) / d;

            /* Resize if necessary */
            if (len == cap) {
                cap *= 2;
                seq_t *tmp = (seq_t *)realloc(period, cap * sizeof(seq_t));
                if (!tmp) { free(period); return (seq_t)0; }
                period = tmp;
            }

            period[len++] = (seq_t)a;

            if (a == A_TERMINATOR)
                break;     /* period ended when 2*A0 is reached */
        }
        period_len = len;
    }

    /* Return result based on index */
    if (n == 0)
        return (seq_t)A0;

    /* Compute index in the periodic part (1-indexed) */
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
