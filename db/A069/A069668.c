/*
 * A069668  Largest nontrivial (no trailing zeros) n-digit square with minimum
 *          digit sum.
 * https://oeis.org/A069668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Largest nontrivial n‑digit square with no trailing zeros and minimal digit sum.
   For odd n≥3: a(n) = (10^{(n‑1)/2} + 1)².
   For even n≥8: a(n) = (5·10^{n/2‑1} + 1)².
   The small even cases are handled explicitly. */
#include <stdbool.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    if (n & 1ULL) {                 /* odd n */
        idx_t k = (n - 1) >> 1;      /* k = (n-1)/2 */
        seq_t t = pow10(k) + (seq_t)1;
        return t * t;
    } else {                        /* even n */
        idx_t k = n >> 1;            /* k = n/2 */
        if (k == 1) return (seq_t)5 * (seq_t)5;             /* n=2 */
        if (k == 2) {                /* n=4, sqrt = 49 */
            seq_t t = pow10(1) * (seq_t)5 - (seq_t)1;
            return t * t;
        }
        if (k == 3) {                /* n=6, sqrt = 549 */
            seq_t t = pow10(2) * (seq_t)5 + (seq_t)49;
            return t * t;
        }
        /* k >= 4 */
        seq_t t = pow10(k - 1) * (seq_t)5 + (seq_t)1;
        return t * t;
    }
}
