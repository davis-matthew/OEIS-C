/*
 * A040562  Continued fraction for sqrt(587).
 * https://oeis.org/A040562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) is the n‑th element of the continued fraction for
 *            sqrt(587). The continued fraction is periodic with period 14:
 *            a(0)=24, the periodic part is (4,2,1,1,1,1,23,1,1,1,1,2,4,48).
 *            The implementation computes this period algorithmically and then
 *            returns a(n) via modular indexing
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Integer square root using binary search; works for 0 <= n < 2^64. */
static idx_t int_sqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid == 0) {
            if (high < 1) break;
            low = 1;
            continue;
        }
        if (mid <= n / mid) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Periodic data for sqrt(587). Compute once. */
    static seq_t period[15];   /* period[0] = a0, period[1..14] = periodic part */
    static bool computed = false;
    static idx_t period_len = 0; /* length of the repeating part (here 14) */

    if (!computed) {
        const idx_t D = 587;          /* the square root under study */
        const idx_t a0 = int_sqrt(D); /* floor(sqrt(587)) = 24 */

        /* Store a0. */
        period[0] = (seq_t)a0;

        /* Compute periodic part using the standard algorithm. */
        idx_t m = 0;
        idx_t d = 1;
        idx_t a_val = a0;
        idx_t i = 1;

        do {
            /* m_n = d_{n-1} * a_{n-1} - m_{n-1} */
            m = d * a_val - m;
            /* d_n = (D - m_n^2) / d_{n-1} */
            d = (D - m * m) / d;
            /* a_n = floor((a0 + m_n) / d_n) */
            a_val = (a0 + m) / d;
            period[i++] = (seq_t)a_val;
        } while (a_val != 2 * a0);  /* last term equals 2*a0 */

        period_len = i - 1; /* number of periodic terms after a0 */
        computed = true;
    }

    /* Return the requested term. */
    if (n == 0)
        return period[0];
    /* For n >= 1, index into the periodic part. */
    return period[1 + ((n - 1) % period_len)];
}
