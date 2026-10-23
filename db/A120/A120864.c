/*
 * A120864  a(n) is the number j for which there exists a unique pair (j,k) of
 *          positive integers such that (j + k + 1)^2 - 4*k = 12*n^2.
 * https://oeis.org/A120864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n^2 - floor(n*sqrt(3))^2
 * Range:     u8 n <= 9, u16 n <= 886, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for any seq_t */
static inline seq_t isqrt_seq_t(seq_t x)
{
    if (x == 0) return 0;
    seq_t lo = 0, hi = x;
    while (lo <= hi) {
        seq_t mid = lo + (hi - lo) / 2;
        if (mid == 0) {                 /* avoid division by zero */
            lo = 1;
            continue;
        }
        if (mid > x / mid) {            /* mid*mid > x  */
            hi = mid - 1;
        } else {                        /* mid*mid <= x */
            seq_t nxt = mid + 1;
            if (nxt > x / nxt)          /* nxt*next > x  */
                return mid;
            lo = nxt;
        }
    }
    return hi;                          /* hi is floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n<1, return 0 */

    seq_t nseq = (seq_t)n;
    seq_t n_sq  = nseq * nseq;          /* n^2   */
    seq_t val   = (seq_t)3 * n_sq;      /* 3*n^2 */
    seq_t m     = isqrt_seq_t(val);     /* floor(n*sqrt(3)) = floor(sqrt(3*n^2)) */

    return val - m * m;                 /* a(n) = 3*n^2 - m^2 */
}
