/*
 * A214319  First differences of A182028.
 * https://oeis.org/A214319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A182028(n+1) - A182028(n) = A182028(n) + A003849(n+1),
 *            where A182028(n) is the binary value of the first n+1 bits of
 *            the Fibonacci word A003849 = 0,1,0,0,1,0,1,0,0,1,... (fixed
 *            point of 0->01, 1->0)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 37 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    size_t need = (size_t)n + 2U;          /* bits w[0..n+1] */
    unsigned char *w = (unsigned char *)malloc(need + 4U);
    if (!w) return 0;
    /* fixed point of morphism 0 -> 01, 1 -> 0, starting from "01" */
    size_t len = 2, i = 1;
    w[0] = 0;
    w[1] = 1;
    while (len < need) {
        if (w[i] == 0) {
            w[len++] = 0;
            w[len++] = 1;
        } else {
            w[len++] = 0;
        }
        i++;
    }
    /* x = A182028(n) = binary value of w[0..n] */
    seq_t x = 0;
    for (size_t k = 0; k <= (size_t)n; ++k) {
        x = (x << 1) + (seq_t)w[k];
    }
    /* A182028(n+1) - A182028(n) = 2x + w[n+1] - x = x + w[n+1] */
    x = x + (seq_t)w[(size_t)n + 1U];
    free(w);
    return x;
}
