/*
 * A036238  Triangle of numbers a(r,j) = j*(j+1) mod r+2, r>=1, j=1..r.
 * https://oeis.org/A036238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy, tabl
 * Formula:   a(n) = j*(j+1) mod (r+2), where r is the row such that r*(r+1)/2
 *            >= n and j = n - r*(r-1)/2
 * Range:     u8 n <= 9936, u16 n <= 9936, u32 n <= 9936, u64 n <= 9936,
 *            u128 n <= 9936, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the row number r for a given linear index n (n >= 1). */
static uint64_t _row_for_n(uint64_t n)
{
    uint64_t low = 1, high = 1;
    while (((__int128)high * (high + 1)) / 2 < n)
        high <<= 1;
    low = high >> 1;
    while (low < high) {
        uint64_t mid = (low + high) >> 1;
        if (((__int128)mid * (mid + 1)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1, but defensive */ return 0;

    uint64_t  un = (uint64_t)n;
    uint64_t  r  = _row_for_n(un);
    uint64_t  prev = (uint64_t)(((__int128)r * (r - 1)) / 2);
    uint64_t  j   = un - prev;
    uint64_t  mod = r + 2;

    unsigned __int128 prod = (unsigned __int128)j * (j + 1);
    uint64_t  res = (uint64_t)(prod % mod);
    return (seq_t)res;
}
