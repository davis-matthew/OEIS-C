/*
 * A186749  a(n) = phi(n - phi(n) + 3).
 * https://oeis.org/A186749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = φ(n - φ(n) + 3)
 * Range:     u8 n <= 406, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 92 terms through n = 9942 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    /* First compute φ(n) */
    seq_t phi_n = totient(n);

    /* Compute the argument of the outer φ: n - φ(n) + 3 */
    seq_t inner = (seq_t)n - phi_n + (seq_t)3;

    /* φ of that argument (cast back to idx_t for the phi call) */
    return totient((idx_t)inner);
}
