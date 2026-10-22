/*
 * A005989  Values B(2,n)/4 of Gandhi polynomials defined by B(x,0)=x and
 *          B(x,n) = x^2 (B(x+1,n-1) - B(x,n-1)).
 * https://oeis.org/A005989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = B(2,n+1)/4 = B(3,n) - B(2,n), where B(x,0) = x and B(x,k)
 *            = x^2*(B(x+1,k-1) - B(x,k-1))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    idx_t len = n + 2; /* x ranges over 2..n+3 */
    seq_t *b = (seq_t *)malloc((size_t)len * sizeof(seq_t));
    idx_t i, k;
    seq_t result;

    for (i = 0; i < len; i++)
        b[i] = (seq_t)(i + 2);

    /* After level k, b[i] = B(i+2, k) is valid for i <= n+1-k */
    for (k = 1; k <= n; k++) {
        idx_t lim = n + 1 - k;
        for (i = 0; i <= lim; i++) {
            seq_t x = (seq_t)(i + 2);
            b[i] = x * x * (b[i + 1] - b[i]);
        }
    }

    result = b[1] - b[0];
    free(b);
    return result;
}
