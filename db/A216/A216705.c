/*
 * A216705  a(n) = Product_{k=1..n} (81 - 9/k).
 * https://oeis.org/A216705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} (81 - 9/k) (a(0)=1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"      /* provides seq_t gcd(seq_t, seq_t) */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;

    seq_t result = (seq_t)1;

    for (idx_t k = 1u; k <= n; ++k) {
        /* numerator = 81*k - 9, denominator = k */
        seq_t numer = (seq_t)81 * (seq_t)k - (seq_t)9;
        seq_t denom = (seq_t)k;

        /* cancel common factor between result and denom */
        seq_t g = gcd(result, denom);
        result /= g;
        denom  /= g;

        /* cancel remaining common factor between numer and denom */
        g = gcd(numer, denom);
        numer  /= g;
        denom  /= g;

        /* denom should now be 1; multiply then divide */
        result *= numer;
        if (denom != (seq_t)1)
            result /= denom;
    }

    return result;
}
