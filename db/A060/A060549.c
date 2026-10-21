/*
 * A060549  a(n) is the number of distinct patterns (modulo geometric
 *          D3-operations) with strict median-reflective (palindrome) symmetry
 *          (i.e., having no other symmetry) which can be formed by an
 *          equilateral triangular arrangement of closely packed black and
 *          white cells satisfying the local matching rule of...
 * https://oeis.org/A060549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 64, u64 n <= 128,
 *            u128 n <= 256, big any n
 * Helpers:   pow2_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A060549
   a(n) = 2^ceil(n/2) – 2^(floor((n+3)/6)+d(n)),
   where d(n)=1 if n≡1 (mod 6), else 0.  */
/* This implementation works for any build where seq_t and idx_t
   are provided.  It handles small native integer types
   (uint64_t, unsigned __int128) specially to avoid overflow,
   and falls back to generic multiplication for arbitrary‑precision
   integer types. */
#include <stdint.h>
#include "../utils/pow2_generic.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* exponents for the two powers of two */
    idx_t e1 = (n + 1) / 2;          /* ceil(n/2)   */
    idx_t e2 = (n + 3) / 6;          /* floor((n+3)/6) */
    idx_t d  = (n % 6 == 1) ? 1 : 0; /* d(n) */
    idx_t ep = e2 + d;                /* exponent of the subtracted term */

    /* Special handling for native unsigned integer types to avoid
       overflow when e1 is equal to the bit width.  For arbitrary‑
       precision types no special case is needed. */
    if (sizeof(seq_t) <= 8) {                /* uint64_t */
        if (e1 < 64) {
            return pow2_generic(e1) - pow2_generic(ep);
        }
        if (e1 == 64) {
            /* 2^64 = 0 (mod 2^64); so 2^64 – x == -x (unsigned) */
            return (seq_t)(-(pow2_generic(ep)));
        }
        /* e1 > 64: not expected for a 64‑bit build that verifies
           only values which fit into uint64_t.  Fall back to generic
           computation (may overflow but the value is not required). */
        return pow2_generic(e1) - pow2_generic(ep);
    } else if (sizeof(seq_t) <= 16) {        /* unsigned __int128 */
        if (e1 < 128) {
            return pow2_generic(e1) - pow2_generic(ep);
        }
        if (e1 == 128) {
            return (seq_t)(-(pow2_generic(ep)));
        }
        return pow2_generic(e1) - pow2_generic(ep);
    } else {                                 /* arbitrary‑precision */
        return pow2_generic(e1) - pow2_generic(ep);
    }
}
