/*
 * A159482  Greatest odd prime q < 2*n such that p < q and p prime and p = 2*n
 *          - q or 0 if no such prime exists.
 * https://oeis.org/A159482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest odd prime q < 2*n such that p = 2*n - q is
 *            prime and p < q. Return 0 if no such prime exists
 * Range:     u8 n <= 129, u16 n <= 32769, u32 n <= 55000, u64 n <= 55000,
 *            u128 n <= 55000, big any n
 * Validated: standard, pass, 82 terms through n = 9184 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;              /* small n have no solutions */

    /* Search odd q descending from 2*n-1 */
    for (idx_t q = 2*n - 1; q >= 3; q -= 2) {
        if (!is_prime(q))            /* q must be prime */
            continue;

        idx_t p = 2*n - q;
        if (p >= q)                  /* need p < q */
            continue;

        if (is_prime(p))
            return (seq_t)q;         /* found the maximal q */
        /* otherwise continue searching lower q */
    }

    return 0;                         /* none found */
}
