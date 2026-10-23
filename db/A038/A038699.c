/*
 * A038699  Riesel problem: Smallest prime of form n*2^m-1, m >= 0, or 0 if no
 *          such prime exists.
 * https://oeis.org/A038699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 147, u64 n <= 147,
 *            u128 n <= 147, big any n
 * Helpers:   isPrimeGeneric, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = smallest prime of the form n*2^m - 1 with m ≥ 0, or 0 if none exists */
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0) {                 /* offset starts at 1, guard for safety */
        return 0;
    }

    /* Maximum exponent allowed without invoking undefined shift behaviour:
       2^m must fit in `seq_t` as a binary power. */
    const idx_t max_m = (idx_t)((8 * sizeof(seq_t)) - 1);

    for (idx_t m = 0; m <= max_m; ++m) {
        seq_t pow = pow2(m);          /* 2^m, safe for all supported seq_t types */

        /* Multiply n by 2^m.  For arbitrary-precision types this is exact.  */
        seq_t prod = (seq_t)n * pow;

        /* Detect overflow for integral types.  If (prod / pow) != n,
           multiplication overflowed and further m will also overflow. */
        if (prod / pow != n) {
            break;
        }

        /* Candidate prime is n*2^m - 1.  Must be >= 2. */
        if (prod <= 1) {
            continue;
        }

        seq_t cand = prod - 1;

        if (cand >= 2 && isPrimeGeneric(cand)) {
            return cand;
        }
    }

    /* No suitable prime found within representable range. */
    return 0;
}
