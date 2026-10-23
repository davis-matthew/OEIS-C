/*
 * A163253  An interspersion: the order array of the odd-numbered columns of
 *          the double interspersion at A161179.
 * https://oeis.org/A163253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) enumerates the array S(m,k) defined by: S(1,k) = k^2 if m
 *            is even: S(m,k) = k^2 + (m-2)k + (m^2-2m+4)/4 if m is odd (m≥3):
 *            S(m,k) = k^2 + (m-2)k + (m^2-2m+1)/4 The order is: list all
 *            pairs (m,k) with m≥1,k≥1, sorted by s=m+k increasing, and within
 *            each s by m increasing (i.e. enumeration alo...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
/*  n is 1‑based.  */
{
    /* Find the diagonal d (sum m+k) such that
       d*(d-1)/2 ≥ n.  */
    idx_t low = 2, high = 2;
    while ((unsigned __int128)high * (high - 1) / 2 < n) {
        high <<= 1;                 /* double until enough */
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid - 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;                 /* diagonal index (sum = d) */

    /* Number of elements before this diagonal */
    unsigned __int128 prev = 0;
    if (d > 2) {
        prev = (unsigned __int128)(d - 2) * (d - 1) / 2;
    }
    idx_t offset = (idx_t)(n - prev);   /* 1‑based offset in diagonal */
    idx_t m = offset;                   /* row index */
    idx_t k = d - offset;               /* column index */

    /* Compute S(m,k) */
    if (m == 1) {
        seq_t kk = (seq_t)k;
        return kk * kk;
    }

    seq_t kk = (seq_t)k;
    seq_t mm = (seq_t)m;
    seq_t two = (seq_t)2;
    seq_t part1 = kk * kk;
    seq_t part2 = (mm - two) * kk;      /* (m-2)k */

    seq_t part3;
    if ((m & 1ULL) == 0) {            /* m even */
        part3 = ((mm * mm) - (mm * two) + (seq_t)4) / (seq_t)4;
    } else {                          /* m odd, m≥3 */
        part3 = ((mm * mm) - (mm * two) + (seq_t)1) / (seq_t)4;
    }

    return part1 + part2 + part3;
}
