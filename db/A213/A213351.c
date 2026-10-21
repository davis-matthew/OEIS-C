/*
 * A213351  9-quantum transitions in systems of N >= 9 spin 1/2 particles, in
 *          columns by combination indices.
 * https://oeis.org/A213351
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^(N-9-2*k) * binomial(N,k) * binomial(N-k, 9+k) where
 *            the (N,k) pair is obtained from the flattened triangle of
 *            A213351 (q = 9)
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 58, u64 n <= 222,
 *            u128 n <= 921, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For n below the sequence offset (9) behaviour is irrelevant. */
    if (n < 9)
        return 0;

    /* d is the zero‑based index within the flattened triangle. */
    idx_t d = n - 9;

    /* Find the row r (0‑based, with N = 9 + r) such that
       S(r-1) <= d < S(r), where S(r) is the cumulative number of
       terms up to row r. */
    idx_t r = 0;
    idx_t s_prev = 0;
    idx_t s = 0;
    while (1) {
        idx_t m = r / 2;          /* floor(r/2) */
        if ((r & 1) == 0) {
            /* r even: S(r) = (m+1)^2 */
            s = (m + 1) * (m + 1);
        } else {
            /* r odd:  S(r) = (m+1)(m+2) */
            s = (m + 1) * (m + 2);
        }
        if (d < s)
            break;
        s_prev = s;
        ++r;
    }

    /* k is the column index within the found row. */
    idx_t k = d - s_prev;

    /* Compute the formula components. */
    idx_t N = 9 + r;                 /* physical N in the triangle */
    idx_t e = r - 2 * k;             /* exponent: N-9-2*k = r-2*k */

    seq_t val = pow2(e);              /* 2^(r-2*k) */
    val = val * binomial(N, k);      /* * binomial(N, k)   */
    val = val * binomial(N - k, 9 + k); /* * binomial(N-k, 9+k) */

    return val;
}
