/*
 * A061435  a(n) is the largest n-digit cube.
 * https://oeis.org/A061435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow_int, safe_pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A061435: a(n) = largest n‑digit cube.
   Formula: a(n) = (floor( (10^n − 1)^(1/3) ))^3 */
#include <stdbool.h>
#include "../utils/pow_int.h"
#include "../utils/safe_pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; value undefined below */

    /* 1. Compute 10^n; if it does not fit in seq_t, the answer does not
       fit either, so return 0. */
    bool overflow;
    seq_t pow10_n = safe_pow10(n, &overflow);
    if (overflow || pow10_n == 0) return 0;

    /* 2. Largest n‑digit number is 10^n − 1. */
    seq_t limit = pow10_n - 1;        /* safe because pow10_n >= 10 */

    /* 3. Bound for cube root: 10^ceil(n/3) is certainly larger than the
       true cube root.  Since n is small when pow10_n fits, this bound
       stays well within seq_t limits. */
    idx_t exp = (n + 2) / 3;          /* ceil(n/3) */
    seq_t high = safe_pow10(exp, &overflow);
    if (overflow) return 0;          /* should not happen when pow10_n fits */

    /* 4. Binary search for floor cube root of limit. */
    seq_t low = 0;
    seq_t best = 0;
    while (low <= high) {
        seq_t mid = (low + high) / 2;
        seq_t mid2 = mid * mid;            /* mid^2 fits in seq_t */
        seq_t mid3 = mid2 * mid;           /* mid^3 fits in seq_t */

        if (mid3 <= limit) {
            best = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;           /* avoid infinite loop */
            high = mid - 1;
        }
    }

    /* 5. Return the cube of the floor cube root. */
    return best * best * best;
}
