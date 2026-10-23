/*
 * A194743  Number of k such that {k*sqrt(5)} > {n*sqrt(5)}, where { } =
 *          fractional part.
 * https://oeis.org/A194743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{1 ≤ k < n : {k*sqrt(5)} > {n*sqrt(5)}}
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses a fixed‑point approximation of sqrt(5) with scaling
   factor 10^12.  The integer part of sqrt(5)*10^12 is 2236067977523.
   For each k we compute (k*2236067977523) mod 10^12 to obtain the
   fractional part of k*sqrt(5) scaled by 10^12.  The comparison of
   scaled fractional parts is exact for all n ≤ 10⁴. */

#include <stdint.h>

static const uint64_t SCALE = 1000000000000ULL;            /* 10^12   */
static const uint64_t SCALED_SQRT5 = 2236067977523ULL;     /* floor(√5 * SCALE) */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* Compute scaled fractional part of n*sqrt(5) */
    const uint64_t fn = (uint64_t)(((__uint128_t)n * SCALED_SQRT5) % SCALE);

    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        const uint64_t fk = (uint64_t)(((__uint128_t)k * SCALED_SQRT5) % SCALE);
        if (fk > fn)
            ++count;
    }
    return count;
}
