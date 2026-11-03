/*
 * A080400  Largest squarefree number dividing phi(n).
 * https://oeis.org/A080400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = rad(phi(n)) (largest square‑free divisor of φ(n))
 * Range:     u8 n <= 261, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 105 terms through n = 9735 (2026-11-01)
 * Helpers:   radical, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/radical.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* safety for indices < 1 */

    seq_t phi_n = totient(n);                 /* φ(n) */
    idx_t phi_val = (idx_t)phi_n;         /* φ(n) fits in idx_t for requested limits */
    return radical(phi_val);                  /* largest square‑free divisor of φ(n) */
}
