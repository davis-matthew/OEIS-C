/*
 * A052465  a(n) is the smallest positive integral solution k to 24*k == 1
 *          (mod 11^n).
 * https://oeis.org/A052465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A052465 – a(n) is the smallest positive integer k with 24*k == 1 (mod 11^n).
 *
 *    Recurrence (for n >= 3):
 *      a(1) = 6
 *      a(2) = 116
 *      a(n) = a(n-1) + 10 * 11^(n-1)        if n is even
 *      a(n) = a(n-1) + 5  * 11^(n-1)        if n is odd
 *
 *    The formula in closed form is:
 *      a(n) = (1 + 23 * 11^n)/24   for even n
 *      a(n) = (1 + 13 * 11^n)/24   for odd  n
 *
 *    This implementation uses the recurrence, which is exact for all arguments
 *    that fit in the underlying seq_t type.  For small types (eg. uint64_t)
 *    the intermediate powers 11^(n-1) never overflow for any n whose
 *    a(n) still fits in the type (the largest such n is 18).  For larger
 *    seq_t types (arbitrary-precision) the multiplication grows as needed.
 */

#include <stdint.h>   /* for uintptr_t etc. – harmless for all builds */

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; undefined for 0, return 0 */
        return (seq_t)0;

    seq_t power_prev = (seq_t)1; /* 11^(i-1) */
    seq_t result     = (seq_t)0;

    for (idx_t i = 1; i <= n; ++i) {
        if (i == 1) {
            result = (seq_t)6;          /* a(1) */
        } else {
            /* inc = (i even ? 10 : 5) * 11^(i-1) */
            seq_t inc = ((i & (idx_t)1U) == 0)
                        ? ((seq_t)10u * power_prev)
                        : ((seq_t)5u  * power_prev);
            result += inc;
        }
        /* update power_prev for next iteration: 11^i */
        power_prev *= (seq_t)11u;
    }

    return result;
}
