/*
 * A055304  Number of labeled rooted trees with n nodes and 3 leaves.
 * https://oeis.org/A055304
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n-3)*(n-2)*(n-1)*(3*n-8)/144, for n ≥ 4
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   cancel_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/cancel_factor.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;               /* behaviour below the offset does not matter */

    /* 144 = 2⁴ · 3² */
    int rem2 = 4;                       /* remaining 2‑factors to cancel */
    int rem3 = 2;                       /* remaining 3‑factors to cancel */
    seq_t result = 1;

    /* Multiply by n! while cancelling factors of 2 and 3 as soon as they appear. */
    for (idx_t i = 1; i <= n; ++i) {
        result *= (seq_t)i;
        cancel_factor(&result, 2, &rem2);
        cancel_factor(&result, 3, &rem3);
    }

    /* Multiply by the extra factors of the formula. */
    result *= (seq_t)(n - 3);
    cancel_factor(&result, 2, &rem2);
    cancel_factor(&result, 3, &rem3);

    result *= (seq_t)(n - 2);
    cancel_factor(&result, 2, &rem2);
    cancel_factor(&result, 3, &rem3);

    result *= (seq_t)(n - 1);
    cancel_factor(&result, 2, &rem2);
    cancel_factor(&result, 3, &rem3);

    result *= ((seq_t)3 * (seq_t)n) - 8;
    cancel_factor(&result, 2, &rem2);
    cancel_factor(&result, 3, &rem3);

    /* Any remaining factors of 2 or 3 should now be cancelled. */
    while (rem2 > 0) { result /= 2; rem2--; }
    while (rem3 > 0) { result /= 3; rem3--; }

    return result;
}
