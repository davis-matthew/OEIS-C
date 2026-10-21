/*
 * A231290  Numbers k dividing u(k), where the Lucas sequence is defined u(i)
 *          = u(i-1) - 5*u(i-2) with initial conditions u(0)=0, u(1)=1.
 * https://oeis.org/A231290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   mul_mod_idx, u_mod_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A231290
    a(n) is the n‑th smallest positive integer k such that k divides
    the Lucas sequence term u_k defined by
        u_0 = 0,   u_1 = 1,
        u_i = u_{i-1} - 5 * u_{i-2}.
    For n = 0 the first term is 1.
*/

#include <stdint.h>
#include "../utils/mul_mod_idx.h"
#include "../utils/u_mod_idx.h"

/*  The required function.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;      /* a(0) = 1 */

    idx_t count = 0;
    /* the first term (k = 1) is already handled, start checking from k = 2 */
    for (idx_t k = 2; ; ++k) {
        if (u_mod_idx(k) == 0) {      /* k divides u_k */
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
