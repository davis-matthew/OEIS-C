/*
 * A173479  a(n) = the smallest n-digit number ending in n-1 zeros that is
 *          divisible by n, else 0.
 * https://oeis.org/A173479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 39,
 *            big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the smallest n‑digit number ending in n‑1 zeros that is divisible by n,
   or 0 if no such number exists.  The algorithm uses a modular test to avoid
   overflowing intermediate products and computes the full value only when the
   result is guaranteed to fit in seq_t. */
#include <stdbool.h>
#include "../utils/mod_mul.h"

seq_t a(idx_t n)
{
    /* n is positive (offset 1).  Use unsigned 64‑bit arithmetic for modulo.*/
    uint64_t nn = (uint64_t)n;
    if (nn == 0) return 0;                /* safety, should not occur */

    /* Compute 10^(n‑1) mod n.  We use mod_mul to avoid overflow.            */
    uint64_t pow10_mod = 1 % nn;
    for (uint64_t i = 1; i < nn; ++i) {
        pow10_mod = mod_mul(pow10_mod, 10, nn);
    }

    /* Test first digits 1…9.  If one yields a multiple of n, compute the
       full value.  The full value fits because we only compute it for a
       divisor that is known to produce a number ≤ seq_t max.                */
    for (uint64_t d = 1; d <= 9; ++d) {
        if (mod_mul(pow10_mod, d, nn) == 0) {
            /* pow10 = 10^(n‑1) as a seq_t.  This will not overflow because
               the result fits in seq_t.                                       */
            seq_t pow10 = 1;
            for (idx_t i = 1; i < n; ++i) {
                pow10 *= (seq_t)10;
            }
            return pow10 * (seq_t)d;
        }
    }

    /* No candidate found. */
    return 0;
}
