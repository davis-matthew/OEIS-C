/*
 * A173789  a(n) is the number of (0,1) matrices A=(a_{ij}) of size n X (3n)
 *          such that each row has exactly three 1's and each column has
 *          exactly one 1 and with the restriction that no 1 stands on the
 *          diagonal from a_{11} to a_{22}.
 * https://oeis.org/A173789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k (3n-k)!/(6^(n-k)*2^k) * binomial(n,k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1, values below are irrelevant */
        return 0;

    idx_t three_n = 3 * n;
    seq_t tmag = 1;            /* magnitude of the current term (always positive) */

    /* compute t0 = (3n)! / 6^n  as a product of n blocks of three integers divided by 6 */
    for (idx_t j = 0; j < n; ++j) {
        idx_t a = three_n - 3 * j;
        idx_t b = a - 1;
        idx_t c = b - 1;
        tmag = (tmag * (seq_t)a * (seq_t)b * (seq_t)c) / (seq_t)6;
    }

    seq_t sum = tmag;           /* first term (k = 0) is positive */
    bool pos = true;            /* current sign of the term stored in tmag */

    /* iterate for k = 0 .. n-1 to generate terms 1 .. n */
    for (idx_t k = 0; k < n; ++k) {
        /* magnitude of the next term: t_{k+1} = t_k * (n-k)*3 / ((k+1)*(3n-k)) */
        seq_t num = (seq_t)(n - k) * (seq_t)3;
        seq_t den = (seq_t)(k + 1) * (seq_t)(three_n - k);
        seq_t nextmag = (tmag * num) / den;

        pos = !pos;                 /* sign alternates each step */

        if (pos)
            sum += nextmag;         /* add if sign is positive */
        else
            sum -= nextmag;         /* subtract if sign is negative */

        tmag = nextmag;             /* prepare for next iteration */
    }

    return sum;
}
