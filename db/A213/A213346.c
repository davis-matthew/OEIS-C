/*
 * A213346  4-quantum transitions in systems of N>=4 spin 1/2 particles, in
 *          columns by combination indices.
 * https://oeis.org/A213346
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   * a(n) = flatten(T(4;N,k)) for N≥4, k=0..⌊(N-4)/2⌋ * T(4;N,k) =
 *            2^(N-4-2k) * binomial(N,k) * binomial(N-k,4+k) * Indices start
 *            at n = 4 (offset 4). a(4)=1, a(5)=10, a(6)=60, etc
 * Range:     u8 n <= 7, u16 n <= 20, u32 n <= 69, u64 n <= 266,
 *            u128 n <= 1072, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k);   */
#include "../utils/pow2.h"       /* seq_t pow2(idx_t exp);             */

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 4.  Behaviour for smaller n is not required. */
    if (n < 4)
        return (seq_t)0;

    idx_t idx = n - 4;      /* zero‑based position in the flattened triangle   */
    idx_t cum = 0;          /* terms before the current row                    */
    idx_t N   = 4;          /* current row index                               */

    while (1) {
        /* Number of terms in row N: k = 0 .. ⌊(N-4)/2⌋ */
        idx_t count = (N - 4) / 2 + 1;

        if (cum + count > idx) {
            idx_t k = idx - cum;           /* column within the row            */

            seq_t term = pow2(N - 4 - 2 * k);
            term *= binomial(N,           k);
            term *= binomial(N - k,       4 + k);

            return term;
        }
        cum += count;
        ++N;
    }
}
