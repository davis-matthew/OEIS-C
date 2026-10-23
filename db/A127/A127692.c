/*
 * A127692  Expansion of psi(x^4) + x * psi(x^12) in powers of x where psi()
 *          is a Ramanujan theta function.
 * https://oeis.org/A127692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = 4*T_k or n = 12*T_k + 1 for some triangular T_k
 *            = k(k+1)/2, otherwise 0
 * Range:     u8 n <= 3455, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _is_triangular, _sqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/_is_triangular.h"
#include "../utils/_sqrt_seq.h"

seq_t a(idx_t n) {
    /* Check the 4 * triangular condition */
    if ((n & 3) == 0) {                    /* n % 4 == 0 */
        if (_is_triangular(n >> 2))        /* n/4 */
            return 1;
    }
    /* Check the 12 * triangular + 1 condition */
    if ((n % 12) == 1) {
        if (_is_triangular((n - 1) / 12))
            return 1;
    }
    return 0;
}
