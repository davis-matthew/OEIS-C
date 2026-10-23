/*
 * A103217  Hexagonal numbers triangle read by rows:
 *          T(n,k)=(n+1-k)*(2*(n+1-k)-1).
 * https://oeis.org/A103217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (r+1-k)*(2*(r+1-k)-1) where r = floor((sqrt(8*n+1)-1)/2)
 *            (the row index), k = n - r*(r+1)/2 (column within the row)
 * Range:     u8 n <= 65, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t low  = 0;
    idx_t high = 1;

    /* Expand high until the condition fails */
    while (1) {
        seq_t val = (seq_t)high * (high + 1) / 2;
        if (val > (seq_t)n)
            break;
        if (high >= (UINT64_MAX >> 1))   /* avoid overflow when shifting */
            break;
        high <<= 1;
    }

    /* Binary search for the largest r with r(r+1)/2 <= n */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        seq_t val = (seq_t)mid * (mid + 1) / 2;
        if (val <= (seq_t)n)
            low = mid;
        else
            high = mid - 1;
    }

    idx_t r = low;                                            /* row */
    idx_t start = (idx_t)(((seq_t)r * (r + 1)) / 2);           /* first index of row */
    idx_t k = n - start;                                      /* column in the row */

    seq_t x = (seq_t)r + 1 - k;                               /* r+1-k */
    seq_t result = x * (2 * x - 1);                           /* T(r,k) */
    return result;
}
