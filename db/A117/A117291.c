/*
 * A117291  a(n) = phi(n)^(n-phi(n)).
 * https://oeis.org/A117291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n)^(n-phi(n))
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 29, u128 n <= 47,
 *            big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute φ(n) */
    seq_t ph = phi(n);

    /* n-φ(n) fits in idx_t since φ(n) ≤ n for n ≥ 1 */
    idx_t exp_idx = n - (idx_t)ph;
    seq_t exp = (seq_t)exp_idx;

    /* Return φ(n) raised to that exponent */
    return big_pow(ph, exp);
}
