/*
 * A084855  Triangular array, read by rows: T(n,k) = concatenated decimal
 *          representations of k and n, 1<=k<=n.
 * https://oeis.org/A084855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = T(m,k) where m is the unique row such that (m-1)*m/2 < n
 *            <= m*(m+1)/2 and k = n - (m-1)*m/2. T(m,k) concatenates decimal
 *            representation of k with m: T(m,k) = k*10^digits(m) + m
 * Range:     u8 n <= 47, u16 n <= 5015, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"        /* seq_t pow10(idx_t e);            */
#include "../utils/digit_count.h" /* unsigned digit_count(uint64_t n); */

seq_t a(idx_t n)
{
    /* Guard for indices before the sequence offset (not needed by spec) */
    if (n == 0)
        return 0;

    /* Find the row number m via binary search on triangular numbers */
    idx_t low = 1, high = 1;
    /* Expand upper bound until it covers n */
    while (((unsigned __int128)high * (high + 1) / 2) < n)
        high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;

    /* Compute k = offset within the row */
    idx_t start = (unsigned __int128)(m - 1) * m / 2;
    idx_t k = n - start;

    /* Compute the concatenated decimal number */
    unsigned d = digit_count(m);
    seq_t term = (seq_t)k * pow10((idx_t)d) + (seq_t)m;
    return term;
}
