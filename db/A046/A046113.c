/*
 * A046113  Coefficients in expansion of theta_3(q) * theta_3(q^6) in powers
 *          of q.
 * https://oeis.org/A046113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions of i^2 + 6*j^2 = n counted as
 *            ordered pairs (i,j) of integers. The function iterates over j,
 *            checks whether n - 6*j^2 is a perfect square, and counts the
 *            corresponding i solutions
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 64‑bit unsigned values */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            if (n == 0) return 0;
            low = 1;
            continue;
        }
        if (mid <= n / mid) {          /* mid*mid <= n, no overflow */
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;                       /* floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t j = 0; ; ++j) {
        /* compute 6*j*j using 128‑bit to avoid overflow */
        __int128 t = (__int128)6 * j * j;
        if (t > n) break;

        idx_t s = n - (idx_t)t;
        idx_t k = isqrt_u64(s);
        if ((seq_t)k * (seq_t)k == s) {
            seq_t mult_i = (k == 0) ? 1 : 2;
            seq_t mult_j = (j == 0) ? 1 : 2;
            cnt += mult_i * mult_j;
        }
    }
    return cnt;
}
