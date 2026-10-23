/*
 * A128261  a(n) = tau(n) + (n-1)*tau(n-1).
 * https://oeis.org/A128261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n) + (n-1)*tau(n-1)
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _tau_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_tau_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined offset, return 0 for safety */
    if (n == 1) return (seq_t)1;   /* tau(1)=1, (n-1)*tau(0)=0 */

    seq_t tau_n      = _tau_u64((uint64_t)n);
    seq_t tau_n_minus = _tau_u64((uint64_t)(n - 1));
    seq_t term = tau_n + (seq_t)(n - 1) * tau_n_minus;
    return term;
}
