/*
 * A213350  8-quantum transitions in systems of N >= 8 spin 1/2 particles, in
 *          columns by combination indices.
 * https://oeis.org/A213350
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(8;N,k) = 2^(N-8-2k) * binomial(N,k) * binomial(N-k,8+k); This
 *            function returns the flattened triangle value for index n (with
 *            offset 8, i.e., a(8) is the first term)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 57, u64 n <= 222,
 *            u128 n <= 949, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* seq_t binomial(idx_t, idx_t); */
#include "../utils/pow2.h"       /* seq_t pow2(idx_t); */

seq_t a(idx_t n)
{
    /* For n < 8 the sequence is undefined; return 0. */
    if (n < 8) return (seq_t)0;

    /* offset from the first term (a(8)). */
    idx_t m = n - 8;

    /* iterate rows until the position is found. */
    idx_t N = 8;          /* current row number */
    idx_t t = 0;          /* t = N - 8 */

    while (1) {
        idx_t maxk = t / 2;        /* floor((N-8)/2) */
        idx_t count = maxk + 1;    /* number of elements in this row */

        if (m < count) {           /* the desired entry is in this row */
            idx_t k = m;           /* column index */

            seq_t val = pow2(N - 8 - 2 * k);
            val = val * binomial(N, k);
            val = val * binomial(N - k, 8 + k);
            return val;
        }

        m -= count;   /* skip this row */
        N++;          /* move to next row */
        t++;          /* N - 8 increases by 1 */
    }
}
