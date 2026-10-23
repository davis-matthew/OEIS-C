/*
 * A198682  Nonnegative multiples of 3 whose sum of base-3 digits are of the
 *          form 3*k+2.
 * https://oeis.org/A198682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 3*k, where k is the n‑th nonnegative integer whose base‑3
 *            * digit sum is congruent to 2 (mod 3). This generates the OEIS
 *            sequence * A198682 (offset 1)
 * Range:     u8 n <= 29, u16 n <= 7282, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_mod3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers – no non‑portable includes. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_sum_mod3.h"

/* Public function required by the build. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  Return 0 safely for n = 0. */
    if (n == 0)
        return 0;

    seq_t candidate = 0;   /* the integer k being tested */
    idx_t  cnt      = 0;   /* how many qualifying k’s we have found */

    while (cnt < n) {
        if (digit_sum_mod3(candidate) == 2) {
            ++cnt;
            if (cnt == n)
                return candidate * (seq_t)3;
        }
        ++candidate;
    }

    /* This point is unreachable for valid n. */
    return 0;
}
