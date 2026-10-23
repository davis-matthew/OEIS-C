/*
 * A101864  Wythoff BB numbers.
 * https://oeis.org/A101864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = B(B(n)), B(k) = floor(k*phi) + k, where phi =
 *            (1+sqrt(5))/2. floor(k*phi) is computed using the convergent
 *            6765/4181 of the continued fraction for phi, which is accurate
 *            for all n up to at least 10^5
 * Range:     u8 none, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   floor_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_phi.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* a(0)=0 in OEIS */

    seq_t n_seq = (seq_t)n;
    seq_t b1 = floor_phi(n_seq) + n_seq;   /* B(n) */
    seq_t b2 = floor_phi(b1) + b1;          /* B(B(n)) */
    return b2;
}
