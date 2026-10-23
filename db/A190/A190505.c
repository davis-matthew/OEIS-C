/*
 * A190505  n+[nr/s]+[nt/s]+[nu/s]; r=golden ratio, s=r+1, t=r+2, u=r+3.
 * https://oeis.org/A190505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*n - floor(sqrt(5*n*n)) - 2
 * Range:     u8 n <= 53, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root for unsigned __int128
   returns floor(sqrt(x)) */
static inline seq_t isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = x;
    seq_t ans = 0;
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid > x / mid) {
            high = mid - 1;
        } else {
            ans = (seq_t)mid;
            low = mid + 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Only defined for n >= 1; return 0 for n==0 for safety */
    if (n == (idx_t)0) return (seq_t)0;

    /* compute floor(sqrt(5*n*n)) */
    unsigned __int128 nn = (unsigned __int128)n * n;
    unsigned __int128 prod = nn * (unsigned __int128)5;
    seq_t m = isqrt_u128(prod);

    /* compute 7*n - m - 2 using 128‑bit intermediate to avoid overflow */
    unsigned __int128 res128 = (unsigned __int128)7 * n - (unsigned __int128)m - 2;
    return (seq_t)res128;
}
