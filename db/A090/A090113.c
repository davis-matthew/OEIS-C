/*
 * A090113  a(n)=pi(n)*[pi(n)-pi(n-3)] where pi(n)=A000720(n).
 * https://oeis.org/A090113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(n)*(pi(n)-pi(n-3))
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Validated: partial, pass, 85 terms through n = 85 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* behaviour below offset does not matter */
        return 0;

    seq_t pi_n  = prime_pi(n);
    seq_t pi_n3 = 0;
    if (n >= 3)
        pi_n3 = prime_pi(n - 3);

    return pi_n * (pi_n - pi_n3);
}
