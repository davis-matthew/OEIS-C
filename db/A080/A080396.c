/*
 * A080396  Largest squarefree numbers dividing the binomial coefficients
 *          C(n,k) read by row, 0<=k<=n. Squarefree kernel of Pascal triangle.
 * https://oeis.org/A080396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = squarefree kernel of C(r,k) where r = largest integer
 *            with r(r+1)/2 <= n (row number) k = n - r(r+1)/2 (column index)
 * Range:     u8 n <= 69, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

static idx_t legendre(idx_t m, uint64_t p)
{
    idx_t e = 0;
    while (m) {
        m /= p;
        e += m;
    }
    return e;
}

seq_t a(idx_t n)
{
    /* Find the row r containing index n */
    idx_t r = 0;
    /* loop while the next row would still be <= n */
    while (((r + 1) * (r + 2)) / 2 <= n) {
        ++r;
    }
    /* Total terms before row r */
    idx_t cum_before = r * (r + 1) / 2;
    /* Column within the row */
    idx_t k = n - cum_before;

    /* Edge cases: first or last element of a row */
    if (k == 0 || k == r) {
        return (seq_t)1;
    }

    /* Build list of all primes <= r */
    uint64_t *primes = NULL;
    size_t pcnt = 0;
    build_primes_upto(r, &primes, &pcnt);

    seq_t rad = (seq_t)1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        idx_t exp_n  = legendre(r,     p);
        idx_t exp_k  = legendre(k,     p);
        idx_t exp_nk = legendre(r - k, p);
        if (exp_n > exp_k + exp_nk) {
            rad *= (seq_t)p;
        }
    }

    free(primes);
    return rad;
}
