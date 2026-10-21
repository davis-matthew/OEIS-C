/*
 * A204255  Symmetric matrix given by f(i,j)=1+[(i+j) mod 4].
 * https://oeis.org/A204255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 + ((ceil((1 + sqrt(1 + 8*n)) / 2) mod 4))
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset starts at 1; value for 0 not defined */

    /* Binary search for the smallest k such that k*(k-1)/2 >= n */
    idx_t low = 1, high = n + 1;  /* k is always <= n+1 */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 val = (__int128)mid * (mid - 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t k = low;
    seq_t result = 1 + (seq_t)(k % 4);
    return result;
}
