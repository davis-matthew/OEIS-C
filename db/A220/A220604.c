/*
 * A220604  Second inverse function (numbers of columns) for pairing function
 *          A081344.
 * https://oeis.org/A220604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = (t mod 2)*min{t, t² - n + 1} + ((t+1) mod 2)*min{t, n -
 *            (t-1)²}, * where t = floor(sqrt(n-1)) + 1
 * Range:     u8 n <= 9870, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root of an unsigned 64‑bit integer, using binary search. */
static inline idx_t floor_sqrt_idx(idx_t x)
{
    idx_t low = 0, high = ((idx_t)1 << 32);   /* sqrt(2^64−1) < 2^32 */
    while (low < high)
    {
        idx_t mid = low + (high - low + 1) / 2;
        if ((unsigned __int128)mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* n is 1‑indexed */
    idx_t t = floor_sqrt_idx(n - 1) + 1;               /* t = floor(sqrt(n-1)) + 1 */
    seq_t parity_t   = t & 1;                        /* t mod 2 */
    seq_t parity_tp1 = (t + 1) & 1;                   /* (t+1) mod 2 */

    /* first component: min{t, t² - n + 1}   */
    unsigned __int128 t2   = (unsigned __int128)t * t;
    unsigned __int128 diff1 = t2 - n + 1;
    seq_t val1 = diff1 > (unsigned __int128)t ? (seq_t)t : (seq_t)diff1;

    /* second component: min{t, n - (t-1)²}   */
    unsigned __int128 t1_2 = (unsigned __int128)(t - 1) * (t - 1);
    unsigned __int128 diff2 = n - t1_2;
    seq_t val2 = diff2 > (unsigned __int128)t ? (seq_t)t : (seq_t)diff2;

    return parity_t * val1 + parity_tp1 * val2;
}
