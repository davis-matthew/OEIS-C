/*
 * A011844  a(n) = floor(binomial(n,7)/8).
 * https://oeis.org/A011844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(binomial(n,7)/8)
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 110, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 919 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute binomial coefficient n choose k multiplicatively.
   This works for any seq_t that supports + - * / % with integer constants. */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)      /* use symmetry */
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 7)
        return 0;
    seq_t b = binomial(n, 7);
    return b / (seq_t)8;
}
