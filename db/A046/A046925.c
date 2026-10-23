/*
 * A046925  Number of ways to express 2n+1 as p+2q; p, q = 1 or prime.
 * https://oeis.org/A046925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of 2n+1 as p+2q where p,q are
 *            either 1 or prime. Computed by exhaustive search
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

/* external linkage as required */
seq_t a(idx_t n)
{
    /* target odd number M = 2n + 1 */
    seq_t M = (seq_t)n * 2 + 1;

    seq_t one = (seq_t)1;
    seq_t M_minus1 = M - one;

    seq_t count = 0;

    /* iterate over q ≥ 1 while 2q ≤ M-1 */
    for (idx_t q = 1; ; ++q) {
        seq_t two_q = (seq_t)q * 2;
        if (two_q > M_minus1) break;

        /* q admissible if q==1 or prime */
        if (q != 1 && !is_prime(q)) continue;

        seq_t p = M - two_q;
        /* p automatically ≥ 1 due to loop condition */

        if (p == one) {
            ++count;
            continue;
        }

        idx_t p_uint = (idx_t)p;
        if (is_prime(p_uint)) ++count;
    }

    return count;
}
