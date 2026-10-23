/*
 * A185180  Enumeration table T(n,k) by antidiagonals. The order of the list
 *          is symmetrical movement from center to edges diagonal.
 * https://oeis.org/A185180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (i*(i+1) + (j-1)*(j+2*i-4)) / 2 + (j > i ? 2*(j-i)-1 :
 *            0), where i = n - t*(t+1)/2, j = (t*t + 3*t + 4)/2 - n, t =
 *            floor[(-1 + sqrt(8*n-7))/2] (the largest t with t*(t+1)/2 < n).
 *            The function implements the above formula without floating‑point
 *            arithmetic
 * Range:     u8 n <= 253, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t find_t(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri < n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* value for n==0 is undefined; return zero as a safe default */

    idx_t t = find_t(n);

    unsigned __int128 tri_t = (unsigned __int128)t * (t + 1) / 2;
    idx_t i = n - (idx_t)tri_t;

    unsigned __int128 tt = (unsigned __int128)t * t + 3 * (unsigned __int128)t + 4;
    unsigned __int128 half_tt = tt / 2;
    idx_t j = (idx_t)(half_tt - n);

    unsigned __int128 ii = (unsigned __int128)i;
    unsigned __int128 jj = (unsigned __int128)j;

    unsigned __int128 base = (ii * (ii + 1) + (jj - 1) * (jj + 2 * ii - 4)) / 2;
    unsigned __int128 val = base;
    if (jj > ii)
        val += 2 * (jj - ii) - 1;

    return (seq_t)val;
}
