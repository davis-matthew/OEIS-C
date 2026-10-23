/*
 * A186749  a(n) = phi(n - phi(n) + 3).
 * https://oeis.org/A186749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = φ(n - φ(n) + 3)
 * Range:     u8 n <= 406, u16 n <= 9942, u32 n <= 9942, u64 n <= 9942,
 *            u128 n <= 9942, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* First compute φ(n) */
    seq_t phi_n = phi(n);

    /* Compute the argument of the outer φ: n - φ(n) + 3 */
    seq_t inner = (seq_t)n - phi_n + (seq_t)3;

    /* φ of that argument (cast back to idx_t for the phi call) */
    return phi((idx_t)inner);
}
