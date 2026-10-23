/*
 * A160484  Third right hand column of the Beta triangle A160480.
 * https://oeis.org/A160484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -7*a(n-1) -21*a(n-2) -35*a(n-3) -35*a(n-4) -21*a(n-5)
 *            -7*a(n-6) - a(n-7) Initial values: a(1) = -299, a(2) = 2063,
 *            a(3) = -8948, a(4) = 29034, a(5) = -77537, a(6) = 180137, a(7) =
 *            -376946
 * Range:     i8 none, i16 n <= 4, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Pre‑computed first seven terms */
    static const seq_t init[7] = {
        -299,     /* a(1) */
        2063,     /* a(2) */
        -8948,    /* a(3) */
        29034,    /* a(4) */
        -77537,   /* a(5) */
        180137,   /* a(6) */
        -376946   /* a(7) */
    };

    /* For n <= 7 simply return the known value. */
    if (n <= 7)
        return init[n - 1];

    /* Buffer holding the last seven values:
       buf[0] = a(k-7), … , buf[6] = a(k-1) for the current step k. */
    seq_t buf[7];
    for (int i = 0; i < 7; ++i)
        buf[i] = init[i];

    seq_t val = 0;
    for (idx_t k = 8; k <= n; ++k) {
        /* Apply the recurrence using the buffered values. */
        val = -(
                (seq_t)7  * buf[6] +  /* a(k-1) */
                (seq_t)21 * buf[5] +  /* a(k-2) */
                (seq_t)35 * buf[4] +  /* a(k-3) */
                (seq_t)35 * buf[3] +  /* a(k-4) */
                (seq_t)21 * buf[2] +  /* a(k-5) */
                (seq_t)7  * buf[1] +  /* a(k-6) */
                buf[0]                /* a(k-7) */
              );

        /* Shift the buffer left: discard a(k-7) and shift the remaining
           values so that buf[6] becomes a(k) for the next iteration. */
        for (int i = 0; i < 6; ++i)
            buf[i] = buf[i + 1];
        buf[6] = val;
    }
    return val;
}
