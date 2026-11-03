/*
 * A117295  a(n) = phi(n)*(n - phi(n)).
 * https://oeis.org/A117295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(n) * (n - phi(n))
 * Range:     u8 n <= 31, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1; return 0 for 0 just to be safe */
    if (n == 0)
        return 0;

    seq_t phi_n = totient(n);          /* φ(n) */
    seq_t diff  = (seq_t)n - phi_n;/* n - φ(n) */
    return phi_n * diff;          /* φ(n) * (n - φ(n)) */
}
