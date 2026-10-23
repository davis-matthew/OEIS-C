/*
 * A044187  Numbers n such that string 0,0 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 64 * b where b is the nth positive integer such that the
 *            base‑8 representation of (b‑1) contains no consecutive zeros.
 *            Thus a(n) is the nth integer whose base‑8 representation has
 *            "00" as a substring but the preceding integer does not
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   has_consecutive_zero_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_consecutive_zero_base8.h"

seq_t a(idx_t n)
{
    /* According to the OEIS offset, n>=1.  For n==0 we return 0,
       which is never used in the test harness. */
    if (n == 0)
        return (seq_t)0;

    idx_t count = 0;
    idx_t candidate = 1;          /* candidate for the factor k in 64*k */

    while (count < n) {
        /* candidate is a positive integer k; we must check that
           (k-1) has no "00" in its base‑8 representation. */
        if (!has_consecutive_zero_base8(candidate - 1)) {
            ++count;
            if (count == n)
                return (seq_t)64 * (seq_t)candidate;
        }
        ++candidate;
    }

    /* Control never reaches here.  Return 0 to silence compiler warnings. */
    return (seq_t)0;
}
