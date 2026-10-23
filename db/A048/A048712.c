/*
 * A048712  2nd column of Family 1 "90 X 150 array": generations 0 .. n of
 *          Rule 150 starting from seed pattern 5.
 * https://oeis.org/A048712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = integer value of (1 + x + x^2)^n * (1 + x^2) modulo 2
 *            (obtained by Rule 150 starting from seed pattern 5)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Size needed: coefficients of (1+x+x^2)^n (degree ≤ 2n)
       plus two extra indices for the (1+x^2) multiplication. */
    size_t len = (size_t)(2 * n + 3);
    unsigned char *c = (unsigned char *)malloc(len);
    if (!c) return 0;                      /* allocation failure – unlikely */
    memset(c, 0, len);
    c[0] = 1;                               /* initial polynomial = 1 */

    idx_t cur_max = 0;                      /* current highest degree */
    for (idx_t step = 1; step <= n; ++step) {
        cur_max += 2;                       /* degree increases by 2 each step */
        for (idx_t k = cur_max; ; --k) {
            unsigned char val = c[k];
            if (k >= 1) val ^= c[k - 1];
            if (k >= 2) val ^= c[k - 2];
            c[k] = val;
            if (k == 0) break;
        }
    }

    /* Build the final integer from the polynomial after multiplying by (1 + x^2) */
    seq_t result = 0;
    idx_t max_exp = 2 * n + 2;              /* highest exponent that may be non‑zero */
    for (idx_t k = max_exp; ; --k) {
        unsigned char bit = c[k];
        if (k >= 2) bit ^= c[k - 2];
        result = (result << 1) | (seq_t)bit;
        if (k == 0) break;
    }

    free(c);
    return result;
}
