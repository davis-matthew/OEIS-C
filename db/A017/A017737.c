/*
 * A017737  Binomial coefficients C(n,73).
 * https://oeis.org/A017737
 *
 * Offset:    73    (a(73) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 75, u32 n <= 80, u64 n <= 91, u128 n <= 132,
 *            big any n
 * Helpers:   gcd, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = binomial(n,73)
 * The function computes the binomial coefficient C(n,73) using only
 * integer arithmetic with arbitrary precision support.  It uses the
 * utilities gcd and seq_mul to avoid overflow for the common case
 * where seq_t is a 64‑bit unsigned integer and to work correctly
 * with arbitrary‑precision builds.
 */
#include "../utils/gcd.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n < 73)
        return 0;   /* behaviour below the OEIS offset is unspecified */

    const idx_t k = 73;
    seq_t result = 1;

    for (idx_t i = 1; i <= k; ++i)
    {
        /* numerator part: n - k + i */
        idx_t numer_idx = n - k + i;
        seq_t numer = (seq_t)numer_idx;

        /* denominator part: i */
        seq_t denom = (seq_t)i;

        /* reduce numerator and denominator by their gcd */
        seq_t g1 = gcd(numer, denom);
        numer /= g1;
        denom /= g1;

        /* further reduce result and denom to keep it integer */
        seq_t g2 = gcd(result, denom);
        result /= g2;
        denom /= g2;

        /* multiply result by numer, then divide by remaining denom */
        result = seq_mul(result, numer);
        if (denom != 1)
            result /= denom;
    }

    return result;
}
