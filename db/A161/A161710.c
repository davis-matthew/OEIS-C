/*
 * A161710  a(n) = (-6*n^7 + 154*n^6 - 1533*n^5 + 7525*n^4 - 18879*n^3 +
 *          22561*n^2 - 7302*n + 2520)/2520.
 * https://oeis.org/A161710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = C(n,0) + C(n,1) + C(n,4) - 3*C(n,5) + 8*C(n,6) -
 *            12*C(n,7)
 * Range:     i8 n <= 9, i16 n <= 14, i32 n <= 54, i64 n <= 1217,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binom_u64(idx_t n, int k)
{
    if (k < 0 || (idx_t)k > n) return 0;
    if (k == 0 || (idx_t)k == n) return 1;
    if (k > (int)(n - k)) k = (int)(n - k);
    seq_t res = 1;
    for (int i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t nC4 = binom_u64(n, 4);
    seq_t nC5 = binom_u64(n, 5);
    seq_t nC6 = binom_u64(n, 6);
    seq_t nC7 = binom_u64(n, 7);

    seq_t result = 1                  /* C(n,0) */
                 + (seq_t)n           /* C(n,1) */
                 + nC4
                 - 3 * nC5
                 + 8 * nC6
                 - 12 * nC7;

    return result;
}
