/*
 * A064848  Period of continued fraction for sqrt(2)*n.
 * https://oeis.org/A064848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = period of the continued fraction expansion of sqrt(2)*n *
 *            (the period ends when the partial quotient equals 2 *
 *            floor(sqrt(2*n^2))). * The implementation uses integers only; a
 *            64‑bit integer type is used for * intermediate calculations so
 *            that the function works for all builds * (seq_t may...
 * Range:     u8 n <= 346, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        uint64_t sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x)  { ans = mid; lo = mid + 1; }
        else         { hi = mid - 1; }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* out of offset, graceful */

    /* Work with 64‑bit intermediates */
    uint64_t N = n;                    /* safe cast, n <= 10 000 for tests */
    uint64_t D = 2 * N * N;            /* D = 2*n^2, fits in 64 bits */

    uint64_t a0 = isqrt_u64(D);        /* floor(sqrt(D)) */

    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;
    uint64_t period = 0;

    /* Standard continued fraction algorithm for sqrt(D) */
    do {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        ++period;
    } while (a != (a0 << 1));          /* stop when a == 2*a0 */

    return (seq_t)period;
}
