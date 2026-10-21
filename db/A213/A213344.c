/*
 * A213344  2-quantum transitions in systems of N>=2 spin 1/2 particles, in
 *          columns by combination indices.
 * https://oeis.org/A213344
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 2^(N-2-2k) * binomial(N,k) * binomial(N-k,2+k) * where
 *            (N,k) are obtained by flattening rows N = 2,3,… with columns * k
 *            = 0…floor((N-2)/2). For integer n ≥ 2, a(n) is the n‑th term of
 *            * OEIS sequence A213344. This implementation computes the value
 *            * directly by evaluating the closed form
 * Range:     u8 n <= 10, u16 n <= 27, u32 n <= 85, u64 n <= 297,
 *            u128 n <= 1136, big any n
 * Helpers:   binomial, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* For n < 2 the sequence is undefined; return 0. */
    if (n < 2) {
        return (seq_t)0;
    }

    /* Convert the 1‑based index to a 0‑based offset into the flattened
       triangle.  For n = 2, offset = 0 corresponds to N=2, k=0. */
    idx_t offset = n - 2;

    /* Find the row N such that the cumulative number of entries up to
       that row exceeds the offset.  The number of terms in row N is
       floor(N/2).  This loop runs only O(sqrt(n)) times. */
    idx_t cum = 0;
    idx_t N = 2;
    while (cum + (N / 2) <= offset) {
        cum += N / 2;
        ++N;
    }

    /* Determine the column index k within that row. */
    idx_t k = offset - cum;

    /* Evaluate the formula.  All arithmetic is performed with seq_t
       operations to preserve precision for arbitrary‑size builds. */
    seq_t power = pow2(N - 2 - 2 * k);
    seq_t B1   = binomial(N, k);
    seq_t B2   = binomial(N - k, 2 + k);

    seq_t temp = seq_mul(power, B1);
    return seq_mul(temp, B2);
}
