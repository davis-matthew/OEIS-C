/*
 * A128221  A128174 * A127701.
 * https://oeis.org/A128221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if (L+1-i) is even, else = i, where L = smallest
 *            integer with L(L+1)/2 >= n, i = n - (L-1)L/2. (same as A128221)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint64_t nn = (uint64_t)n;

    /* find L such that L*(L+1)/2 >= n  */
    uint64_t low = 1, high = nn;
    while (low < high) {
        uint64_t mid = low + ((high - low) >> 1);
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t >= nn)
            high = mid;
        else
            low = mid + 1;
    }
    uint64_t L = low;

    uint64_t prev = (L - 1) * L / 2; /* sum of first L-1 diagonals */
    uint64_t i = nn - prev;          /* position inside current diagonal */

    uint64_t parity = (L + 1 - i) & 1;
    if (parity == 0)
        return (seq_t)1;
    else
        return (seq_t)i;
}
