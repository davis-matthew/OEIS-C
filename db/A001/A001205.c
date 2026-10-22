/*
 * A001205  Number of clouds with n points; number of undirected 2-regular
 *          labeled graphs; or number of n X n symmetric matrices with (0,1)
 *          entries, trace 0 and all row sums 2.
 * https://oeis.org/A001205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n-1)*a(n-1) + (n-1)*(n-2)/2*a(n-3), a(0) = 1, a(1) = 0,
 *            a(2) = 0
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    seq_t x0, x1, x2, next;
    idx_t i;

    if (n == 0) return (seq_t)1;
    if (n <= 2) return (seq_t)0;

    x0 = (seq_t)1; /* a(i-3) */
    x1 = (seq_t)0; /* a(i-2) */
    x2 = (seq_t)0; /* a(i-1) */
    for (i = 3; i <= n; i++) {
        idx_t c;
        if ((i - 1) % 2 == 0)
            c = ((i - 1) / 2) * (i - 2);
        else
            c = (i - 1) * ((i - 2) / 2);
        next = (seq_t)(i - 1) * x2 + (seq_t)c * x0;
        x0 = x1;
        x1 = x2;
        x2 = next;
    }
    return x2;
}
