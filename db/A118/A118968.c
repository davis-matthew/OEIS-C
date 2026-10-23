/*
 * A118968  a(4n+k) = (k+1)*binomial(5n+k,n)/(4n+k+1), k=0..3.
 * https://oeis.org/A118968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(4n+k) = (k+1)*binomial(5n+k,n)/(4n+k+1), k = 0..3 (OEIS
 *            A118968)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 35, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Decompose n into 4n + k. */
    idx_t n_div4 = n / 4;
    idx_t k = n % 4;

    /* Compute binomial(5n + k, n).  The helper handles arbitrary‑size integers. */
    seq_t bin = binomial(5 * n_div4 + k, n_div4);

    /* numerator = (k+1) * bin */
    seq_t num = (seq_t)(k + 1) * bin;

    /* denominator = 4n + k + 1 */
    seq_t denom = (seq_t)(4 * n_div4 + k + 1);

    /* division is exact by definition of the sequence */
    return num / denom;
}
