/*
 * A091136  Smallest number m such that number of times m divides k! is almost
 *          k/n for large k, i.e., smallest m with A090624(m)=n.
 * https://oeis.org/A091136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{ (p prime) | p-1 divides n } p^(n/(p-1)) Special
 *            case: a(0) = 1 (by definition)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 36 (2026-11-01)
 * Helpers:   ipow_u64_idx, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_u64_idx.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Handle offset */
    if (n == 0)
        return (seq_t)1;

    /* Candidate from p = 2 (always valid) */
    seq_t min_val = ipow_u64_idx(2, n);

    for (idx_t d = 1; d <= n / d; ++d) {   /* iterate over all divisors d of n */
        if (n % d != 0)
            continue;

        /* First divisor d */
        if (is_prime(d + 1)) {
            seq_t val = ipow_u64_idx(d + 1, n / d);
            if (val < min_val)
                min_val = val;
        }

        /* Reciprocal divisor d2 = n/d */
        idx_t d2 = n / d;
        if (d2 != d && is_prime(d2 + 1)) {
            seq_t val2 = ipow_u64_idx(d2 + 1, n / d2);
            if (val2 < min_val)
                min_val = val2;
        }
    }

    return min_val;
}
