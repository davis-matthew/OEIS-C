/*
 * A067751  Number of ordered triples (x,y,z) of nonnegative integers such
 *          that x*y+x*z+y*z = n.
 * https://oeis.org/A067751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of ordered triples (x,y,z) of nonnegative *
 *            integers such that xy + xz + yz = n. * Computed by enumerating
 *            all pairs (x,y) with x ≥ 1; * the cases with x = 0 are handled
 *            separately via divisor counting
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t divisor_count(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            cnt += 1;
            if (d * d != n)
                cnt += 1;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    seq_t result = 0;

    /* Triples with x = 0 :  y * z = n  (y,z > 0) */
    result += divisor_count(n);

    /* Triples with x ≥ 1 */
    for (idx_t x = 1; x <= n; ++x) {
        idx_t max_y = n / x;          /* ensures x*y ≤ n */
        for (idx_t y = 0; y <= max_y; ++y) {
            seq_t xy   = (seq_t)x * y;
            seq_t sum  = (seq_t)x + y;
            seq_t rem  = (seq_t)n - xy;
            if (rem % sum == 0)        /* z = rem / sum is integer, z ≥ 0 */
                result++;
        }
    }

    return result;
}
