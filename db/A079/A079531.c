/*
 * A079531  a(n) = phi(n) - ceiling(n^(2/3)).
 * https://oeis.org/A079531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = phi(n) - ceil(n^(2/3))
 * Range:     i8 n <= 161, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9617 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the totient function from utils/phi.h and an integer cube‑root routine. */
#include <stdint.h>
#include "../utils/totient.h"

static seq_t ceil_pow_two_over_three(idx_t n)
{
    /* Compute ceil(n^(2/3)) = ceil(cuberoot(n^2)). */
    __int128 sq = (__int128)n * n;          /* n² fits in 128 bits. */
    idx_t lo = 0, hi = n, best = 0;

    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        __int128 mid_cub = (__int128)mid * mid * mid;
        if (mid_cub <= sq) {
            best = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }

    __int128 cube = (__int128)best * best * best;
    return (cube == sq) ? (seq_t)best : (seq_t)(best + 1);
}

seq_t a(idx_t n)
{
    seq_t phi_n = totient(n);
    seq_t ceil23 = ceil_pow_two_over_three(n);
    return phi_n - ceil23;
}
