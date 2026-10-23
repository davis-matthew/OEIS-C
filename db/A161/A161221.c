/*
 * A161221  Consider necklaces with n beads, each black or white, where the n
 *          segments of cord between the beads are each colored red or green;
 *          a(n) is the number of different necklaces under the action of the
 *          dihedral group D_{2n}.
 * https://oeis.org/A161221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 for n > 0: a(n) = (1/2) * [ (1/n) * Σ_{d|n} φ(n/d) *
 *            2^(2d) + 2^(n+1) ] where φ is Euler's totient function
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   phi, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/pow2.h"

/* Compute the n‑th term of sequence A161221. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;            /* the first term */

    seq_t sum = 0;

    /* Enumerate all positive divisors d of n. */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            seq_t phi_val = phi(n / d);
            seq_t term    = phi_val * pow2(2 * d);
            sum += term;

            /* the paired divisor n/d, if distinct */
            idx_t d2 = n / d;
            if (d2 != d) {
                seq_t phi_val2 = phi(n / d2);
                seq_t term2    = phi_val2 * pow2(2 * d2);
                sum += term2;
            }
        }
    }

    /* (1/n) * sum */
    seq_t val   = sum / (seq_t)n;
    /* 2^(n+1) */
    seq_t part  = pow2(n + 1);

    /* Final division by 2 */
    return (val + part) / (seq_t)2;
}
