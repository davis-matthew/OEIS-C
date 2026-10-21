/*
 * A090112  a(n)=pi(n)*[pi(n)-pi(n-2)] where pi(n)=A000720(n).
 * https://oeis.org/A090112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(n) * ( pi(n) - pi(n-2) )
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n == 0)             /* indices below 1 are irrelevant */
        return 0;

    /* Compute pi(n) and pi(n-2) */
    seq_t pi_n         = compute_pi(n);
    seq_t pi_n_minus_2 = (n > 2) ? compute_pi(n - 2) : (seq_t)0;

    /* a(n) = pi(n) * ( pi(n) - pi(n-2) ) */
    return pi_n * (pi_n - pi_n_minus_2);
}
