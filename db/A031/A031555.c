/*
 * A031555  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 57.
 * https://oeis.org/A031555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * SEQUENCE: A031555
 *
 * Numbers k such that the continued fraction expansion of sqrt(k) has an
 * even period and, after deleting the last term of that periodic part, the
 * central term equals 57.
 */
#include <stdint.h>
#include <stddef.h>

/* Integer square root (floor) for uint64_t */
static uint64_t isqrt_u64(uint64_t n)
{
    if (n < 2) return n;
    uint64_t lo = 0, hi = n >> 1 | 1;
    while (lo < hi) {
        uint64_t mid = (lo + hi) >> 1;
        uint64_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) lo = mid + 1;
        else hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t k = 2;

    while (1) {
        uint64_t a0 = isqrt_u64(k);
        if (a0 * a0 == k) {            /* skip perfect squares */
            k++;
            continue;
        }

        /* continued fraction of sqrt(k) */
        uint64_t m = 0;
        uint64_t d = 1;
        uint64_t a = a0;
        uint64_t terms[2000];
        size_t len = 0;

        while (1) {
            m = d * a - m;
            d = (k - m * m) / d;
            a = (a0 + m) / d;
            terms[len++] = a;
            if (a == 2 * a0)        /* period ends */
                break;
        }

        if ((len % 2 == 0) && (terms[len / 2 - 1] == 57)) {
            count++;
            if (count == n)
                return (seq_t)k;
        }

        k++;
    }
}
