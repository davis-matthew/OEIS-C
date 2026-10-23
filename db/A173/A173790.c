/*
 * A173790  a(n) is the number of (0,1) matrices A=(a_{ij}) of size n X (4n)
 *          such that each row has exactly four 1's and each column has
 *          exactly one 1 and with the restriction that no 1 stands on the
 *          diagonal from a_{11} to a_{22}.
 * https://oeis.org/A173790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * binomial(n,k) * * (4n-k)! /
 *            (24^(n-k) * 6^k) * where 24 = 4! and 6 = 3!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* ------------- compute f0 = (4*n)! / 24^n ------------------ */
    seq_t f0 = 1;
    idx_t total = 4 * n;
    uint64_t rem2 = 3 * n;   /* remove 2^3n from 24^n */
    uint64_t rem3 = n;       /* remove 3^n  from 24^n */

    for (idx_t i = 1; i <= total; ++i) {
        idx_t t = i;
        /* strip 2s up to the remaining count */
        while (rem2 > 0 && (t & 1) == 0) {
            t >>= 1;
            rem2--;
        }
        /* strip 3s up to the remaining count */
        while (rem3 > 0 && t % 3 == 0) {
            t /= 3;
            rem3--;
        }
        f0 *= (seq_t)t;
    }
    /* After the loop rem2 and rem3 should be 0. */

    /* ------------- forward accumulation ----------------------- */
    seq_t sum   = f0;            /* term for k = 0 (positive sign) */
    seq_t bin   = 1;             /* C(n,0) */
    seq_t f     = f0;            /* current f(k) */
    seq_t term  = f0;            /* term(k) = bin * f */

    for (idx_t k = 0; k < n; ++k) {
        /* update binomial coefficient C(n, k+1) */
        bin = bin * (n - k) / (k + 1);

        /* update f(k+1) = f(k) * 4 / (4n - k) */
        f = f * 4 / ((4 * n) - k);

        term = bin * f;

        /* add or subtract term depending on parity of (k+1) */
        if ( ((k + 1) & 1) == 0 )
            sum += term;   /* even index -> positive sign */
        else
            sum -= term;   /* odd index  -> negative sign */
    }

    return sum;
}
