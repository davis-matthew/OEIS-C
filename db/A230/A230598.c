/*
 * A230598  Lexicographically earliest sequence of distinct positive integers
 *          such that all black pixels in the binary plot of the sequence are
 *          connected (see Comments for details).
 * https://oeis.org/A230598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   OEIS A230598 implemented by the recurrence for k > 2: a(2^k-1) =
 *            2^k-1 a(2^k) = 2^k+1 a(n) = a(n-2^k+1)+2^k for 2^k <= n <
 *            2^(k+1)-(k+1) a(n) = 2^k for n = 2^(k+1)-(k+1) a(n) =
 *            a(n-2^k)+2^k for 2^(k+1)-(k+1) < n < 2^(k+1)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"   /* pow2(idx_t) returns seq_t = 2^exp */

static inline idx_t floor_log2_u64(idx_t v)
{
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* Base cases for n <= 7 */
    static const seq_t base[7] = {
        (seq_t)1, (seq_t)3, (seq_t)2, (seq_t)6,
        (seq_t)4, (seq_t)5, (seq_t)7
    };
    if (n <= 7) return base[n-1];

    /* For n >= 8, k = floor_log2(n) >= 3 */
    idx_t k = floor_log2_u64(n);
    idx_t low_idx = 1ULL << k;    /* 2^k as idx_t */
    seq_t low_seq = pow2(k);      /* 2^k as seq_t */

    if (k == 63) {
        /* Special handling to avoid shifting by 64 bits */
        unsigned __int128 high128 = (unsigned __int128)low_idx << 1; /* 2^64 */
        unsigned __int128 mid128  = high128 - (k + 1);              /* 2^64 - 64 */
        idx_t mid_idx = (idx_t)mid128;
        if (n < mid_idx) {
            return a(n - low_idx + 1) + low_seq;
        } else if (n == mid_idx) {
            return low_seq;
        } else {
            return a(n - low_idx) + low_seq;
        }
    }

    idx_t high_idx = low_idx << 1;          /* 2^(k+1) as idx_t */
    idx_t mid_idx  = high_idx - (k + 1);    /* 2^(k+1) - (k+1) */

    if (n < mid_idx) {
        return a(n - low_idx + 1) + low_seq;
    } else if (n == mid_idx) {
        return low_seq;
    } else {
        return a(n - low_idx) + low_seq;
    }
}
