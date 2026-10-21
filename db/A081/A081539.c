/*
 * A081539  Triangle read by rows in which the n-th row contains the n numbers
 *          in increasing order formed by the concatenation of first n-1
 *          numbers. (The digits of the numbers with 2 or more digits are
 *          taken as one entity.) First row is taken to be 0.
 * https://oeis.org/A081539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = concatenation of all integers 1..r except omitted, where
 *            r is the row number such that r*(r+1)/2 ≥ n, and omitted = r -
 *            (n - (r-1)*r/2) + 1
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

static idx_t find_row(idx_t n)
{
    idx_t low = 1;
    idx_t high = 1;
    while ((__int128)high * (high + 1) / 2 < n)
        ++high;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    idx_t r = find_row(n);
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;                     /* position in the row (1‑based) */
    idx_t omitted = r - k + 1;                  /* number omitted from 1..r */

    seq_t val = 0;
    for (idx_t i = 1; i <= r; ++i) {
        if (i == omitted)
            continue;

        /* number of decimal digits of i */
        idx_t digits = 0;
        idx_t t = i;
        do {
            ++digits;
            t /= 10;
        } while (t);

        val = val * pow10(digits) + (seq_t)i;
    }
    return val;
}
