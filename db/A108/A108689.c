/*
 * A108689  Smallest integer q >= 1 such that difference between q*Pi and the
 *          nearest integer is <= 1/n.
 * https://oeis.org/A108689
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal q ≥ 1 such that |q·π − nearest integer| ≤ 1/n *
 *            (offset 2: a(2) is the first term of the sequence). The
 *            algorithm uses * integer arithmetic by representing π as the
 *            rational number * 3141592653589793/1000000000000000 (15 decimal
 *            places). For each * candidate denominator q we comput...
 * Range:     u8 n <= 154, u16 n <= 154, u32 n <= 154, u64 n <= 154,
 *            u128 n <= 154, big any n
 * Helpers:   mul_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_mod.h"

seq_t a(idx_t n)
{
    /* The definition is only relevant for n ≥ 2.  Return 0 for smaller
     * indices – the result is never used. */
    if (n < 2)
        return 0;

    /* π ≈ 3.141592653589793…, represented as an integer ratio
     *   π = PI_NUM / PI_DEN
     * with 15 decimal places of precision (sufficient for n up to at
     * least several millions). */
    const uint64_t PI_NUM = 3141592653589793ULL;          /* 3.141592653589793 * 10^15 */
    const uint64_t PI_DEN = 1000000000000000ULL;          /* 10^15          */

    uint64_t q = 1;
    while (1) {
        /* Compute (q * PI_NUM) mod PI_DEN without overflow. */
        uint64_t prod = mul_mod(q, PI_NUM, PI_DEN);

        /* Distance from q·π to the nearest integer expressed as a
         * numerator over PI_DEN.  The minimal fractional part is
         * either 'prod' or 'PI_DEN - prod'. */
        uint64_t half = PI_DEN >> 1;
        uint64_t min_frac = (prod <= half) ? prod : PI_DEN - prod;

        /* The inequality min_frac / PI_DEN ≤ 1 / n is equivalent to
         * min_frac ≤ PI_DEN / n.  The right‑hand side uses integer
         * division, which is safe because all quantities are positive
         * integers. */
        uint64_t bound = PI_DEN / n;
        if (min_frac <= bound)
            return (seq_t)q;

        ++q;
    }
}
