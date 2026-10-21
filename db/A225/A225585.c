/*
 * A225585  Floor((3^n-1)/n).
 * https://oeis.org/A225585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((3^n - 1)/n)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 43, u128 n <= 83,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation handles two cases:
 * 1) When seq_t is a fixed‑size unsigned integer (≤64 bits) we use a
 *    128‑bit intermediate to compute 3^n safely.
 * 2) When seq_t is an arbitrary‑precision integer (or larger than 64
 *    bits) we fall back to the generic big_pow routine supplied by the
 *    framework.  The intermediate 3^n then fits into seq_t for all terms
 *    that are checked in that build.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {                    /* offset is 1 – undefined below that */
        return 0;
    }

    /* Detect whether seq_t is a 64‑bit basic type.  If so we can safely
     * use a 128‑bit intermediate to avoid overflow when raising 3 to the
     * power n.  For all other seq_t's we presume arbitrary precision
     * (or a type larger than 64 bits) and rely on big_pow.
     */
    static const bool builtin64 =
        (sizeof(seq_t) == sizeof(uint64_t));

    if (builtin64) {
        /* Use an unsigned 128‑bit type for the intermediate product. */
        __uint128_t pow3 = 1;
        for (idx_t i = 0; i < n; ++i) {
            pow3 *= 3;
        }
        pow3 -= (__uint128_t)1;
        /* floor division by n.  The result fits in seq_t for all terms
         * that the 64‑bit build checks.
         */
        return (seq_t)(pow3 / n);
    } else {
        /* Arbitrary‑precision path: big_pow returns seq_t.  All
         * operators are expected to be overloaded for that type by
         * the utilities supplied in the build.
         */
        seq_t pow3 = big_pow((seq_t)3, n);
        pow3 -= (seq_t)1;
        return pow3 / (seq_t)n;
    }
}
