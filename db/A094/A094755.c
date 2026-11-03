/*
 * A094755  Least integer value of (1 + 2^n + 3^n + ... + k^n)/(1 + 2 + 3 +
 *          ... + k), k > 1.
 * https://oeis.org/A094755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least integer value of (1 + 2^n + 3^n + ... + k^n)/(1 + 2
 *            + 3 + ... + k), k > 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 11, u64 n <= 23, u128 n <= 35,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 663 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    /* For odd n (n ≥ 3) the minimal integer value happens at k = 2. */
    if ((n & 1ULL) != 0ULL)
    {
        seq_t pow2n = ipow((seq_t)2, (seq_t)n);
        return (pow2n + 1) / 3;
    }

    seq_t sum = 0;
    for (idx_t i = 1; ; ++i)
    {
        seq_t term = ipow((seq_t)i, (seq_t)n);
        sum += term;

        if (i > 1)
        {
            seq_t den = (seq_t)i * ((seq_t)i + 1);
            den /= 2;                 /* denominator = k(k+1)/2 */
            if (sum % den == 0)
                return sum / den;     /* the least integer value found */
        }
    }
}
