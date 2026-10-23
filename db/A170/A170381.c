/*
 * A170381  Number of reduced words of length n in Coxeter group on 36
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 36, a(2) = 1260, a(3) = 44100, a(4) = 1543500,
 *            a(5) = 54022500, a(6) = 1890787500, a(7) = 66177562500, a(8) =
 *            2316214687500, a(9) = 81067514062500, a(10) = 2837362992187500,
 *            a(11) = 99307704726562500, a(12) = 3475769665429687500, a(13) =
 *            121651938290039062500, a(14) = 42578178401...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Allocate a small circular buffer for the last 44 terms */
    seq_t *buf = (seq_t *)malloc(44 * sizeof(seq_t));
    if (!buf) return 0;  /* allocation failure, return 0 as a safe value */

    /* Initialize buffer to zero (covering indices < 0) */
    for (size_t i = 0; i < 44; ++i)
        buf[i] = 0;

    seq_t sum42 = 0;      /* Σ_{i=1}^{42} a(n-i) for the current n */
    seq_t result = 0;     /* Will hold a[n] on the final iteration */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t rhs;
        if (i == 0)
            rhs = 1;
        else if (i <= 42)
            rhs = 2;
        else if (i == 43)
            rhs = 1;
        else
            rhs = 0;

        /* Compute a(i) = rhs + 34*sum42 - 595*a(i-43) if i>=43 */
        seq_t term = rhs + sum42 * (seq_t)34;
        if (i >= 43)
            term -= buf[(i - 43) % 44] * (seq_t)595;

        /* Store term in the circular buffer */
        buf[i % 44] = term;

        /* Update sum42 for the next iteration */
        sum42 += term;
        if (i >= 42)
            sum42 -= buf[(i - 42) % 44];

        if (i == n)
            result = term;
    }

    free(buf);
    return result;
}
