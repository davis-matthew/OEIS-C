/*
 * A213352  10-quantum transitions in systems of N >= 10 spin 1/2 particles,
 *          in columns by combination indices.
 * https://oeis.org/A213352
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(10;N,k) where * T(10;N,k) = 2^(N-10-2*k) *
 *            binomial(N,k) * binomial(N-k,10+k) * N and k are chosen from the
 *            flattened triangle: * let offset = n-10, * find the smallest N ≥
 *            10 such that offset < floor((N-10)/2)+1, * then k = offset. This
 *            gives the n‑th term for n ≥ 10
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 52, u64 n <= 209,
 *            u128 n <= 919, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 10) return 0;          /* behaviour for n < 10 is unspecified */

    idx_t offset = n - 10;         /* zero‑based index in the flattened triangle */
    idx_t N = 10;                  /* current row number */

    /* Find the row N that contains this offset */
    while (1) {
        idx_t row_len = (N - 10) / 2 + 1;   /* floor((N-10)/2)+1 */
        if (offset < row_len) break;
        offset -= row_len;
        ++N;
    }

    /* Column index within this row */
    idx_t k = offset;

    /* Compute the formula */
    idx_t exp = N - 10 - 2 * k;    /* exponent for the power of 2, always ≥ 0 */
    seq_t result = pow2(exp);      /* 2^(N-10-2*k) */
    result *= binomial(N, k);      /* multiply by binomial(N,k) */
    result *= binomial(N - k, 10 + k);  /* multiply by binomial(N-k,10+k) */

    return result;
}
