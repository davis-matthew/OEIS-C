/*
 * A053414  Circle numbers (version 2): a(n) is the number of points (i,j),
 *          i,j integers, contained in a circle of diameter n, centered at (0,
 *          1/2).
 * https://oeis.org/A053414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of integer lattice points (i,j) inside the circle
 *            centered at (0,1/2) with diameter n. The implementation uses an
 *            integer square root function to keep everything exact
 * Range:     u8 n <= 18, u16 n <= 288, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for unsigned 64-bit values, using binary search.  */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)  { ans = mid; low = mid + 1; }
        else         { high = mid - 1; }
    }
    return ans;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_i = n / 2;        /* floor(n/2) */

    for (idx_t i = 0; i <= max_i; ++i) {
        /* D = n² - 4·i², all values are non‑negative for this range. */
        idx_t D = n * n - 4 * i * i;
        idx_t t = isqrt_u64(D);            /* floor(sqrt(D))                 */
        seq_t oddcount = ((t & 1) ? (seq_t)(t + 1) : (seq_t)t);  /* count odd m in [-t,t] */

        if (i == 0)
            sum += oddcount;               /* i = 0 counted once            */
        else
            sum += oddcount << 1;          /* i > 0 and i < 0 are symmetric */
    }

    return sum;
}
