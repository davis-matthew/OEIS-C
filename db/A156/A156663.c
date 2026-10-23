/*
 * A156663  Triangle by columns, powers of 2 interleaved with zeros.
 * https://oeis.org/A156663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r be the unique integer with r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.
 *            Let k = n - r*(r+1)/2 (the position in row r, 0 ≤ k ≤ r). Then
 *            a(n) = 0 if (r - k) is odd, otherwise a(n) = 2^((r - k)/2)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2    */
    idx_t low  = 0;
    idx_t high = 1;

    /* Expand high until it exceeds the target */
    while (((unsigned __int128)high * (high + 1) / 2) <= n) {
        if (high > UINT64_MAX / 2) break;     /* avoid overflow */
        high <<= 1;
    }

    /* Binary search for the largest r with r*(r+1)/2 ≤ n */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (((unsigned __int128)mid * (mid + 1) / 2) <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;

    /* Position k within the row */
    idx_t start = (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
    idx_t k     = n - start;          /* 0 ≤ k ≤ r */

    idx_t d = r - k;                  /* difference */

    if (d & 1)                         /* odd -> zero */
        return 0;

    idx_t exp = d >> 1;                /* (r - k)/2 */
    return pow2(exp);
}
