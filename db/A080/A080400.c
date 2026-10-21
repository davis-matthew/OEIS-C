/*
 * A080400  Largest squarefree number dividing phi(n).
 * https://oeis.org/A080400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = rad(phi(n)) (largest square‑free divisor of φ(n))
 * Range:     u8 n <= 261, u16 n <= 9735, u32 n <= 9735, u64 n <= 9735,
 *            u128 n <= 9735, big any n
 * Helpers:   phi, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/rad.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* safety for indices < 1 */

    seq_t phi_n = phi(n);                 /* φ(n) */
    idx_t phi_val = (idx_t)phi_n;         /* φ(n) fits in idx_t for requested limits */
    return rad(phi_val);                  /* largest square‑free divisor of φ(n) */
}
