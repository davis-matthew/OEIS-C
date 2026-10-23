/*
 * A154631  Triangle read by rows: T(m,n) = 2mn + m + n + 5.
 * https://oeis.org/A154631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2 m j + m + j + 5, where m = smallest k with k(k+1)/2 ≥ n
 *            and j = n - (m-1)m/2. (Triangle read by rows)
 * Range:     u8 n <= 65, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find row m such that m(m+1)/2 >= n */
    uint64_t low = 1, high = 1;
    /* Exponentially increase high until the condition holds */
    while ((unsigned __int128)high * (high + 1) / 2 < n) {
        high <<= 1;
        if (high == 0) { /* overflow, use maximum 64‑bit value */
            high = UINT64_MAX;
            break;
        }
    }
    /* Binary search between low and high */
    while (low < high) {
        uint64_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    uint64_t m = low;

    /* Compute first index of row m */
    unsigned __int128 start = (unsigned __int128)(m - 1) * m / 2;
    uint64_t j = n - (uint64_t)start;           /* column index (1‑based) */

    /* Compute the value using seq_t calculations */
    seq_t sm = (seq_t)m;
    seq_t sj = (seq_t)j;
    seq_t val = (seq_t)2 * sm * sj + sm + sj + (seq_t)5;
    return val;
}
