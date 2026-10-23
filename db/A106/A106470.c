/*
 * A106470  Inverse of number triangle A106468.
 * https://oeis.org/A106470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   Let r = ⌊(√(8n+1)−1)/2⌋, c = n−r(r+1)/2, m = r−c. If c is odd
 *            and m is odd → 0; else if c is even and m is even → (−1)^{m/2};
 *            else if c is even and m is odd → (−1)^{(m+1)/2}; else (−1)^{m/2}
 * Range:     i8 n <= 97, i16 n <= 97, i32 n <= 97, i64 n <= 97, i128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root for numbers up to 2^68 (sufficient for 8*n+1) */
static inline uint64_t isqrt_uint128(unsigned __int128 x)
{
    uint64_t low = 0;
    uint64_t high = ((uint64_t)1U << 34) + 1;  /* sqrt(2^68) < 2^34+1 */
    uint64_t mid;
    while (low <= high) {
        mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* compute a(n) from triangle definition */
seq_t a(idx_t n)
{
    /* compute row index r */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    uint64_t s = isqrt_uint128(val);
    uint64_t r = (s - 1) >> 1;  /* floor((sqrt(8n+1)-1)/2) */

    /* adjust if overshoot (should not happen but safe) */
    while (((unsigned __int128)r * (r + 1)) / 2 > n) r--;
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n) r++;

    /* column index */
    uint64_t row_start = ((unsigned __int128)r * (r + 1)) / 2;
    uint64_t c = n - row_start;

    /* m = r - c */
    uint64_t m = r - c;

    /* determine value */
    seq_t val_out = 0;

    if (c & 1) {      /* c odd */
        if ((m & 1) == 0) {   /* m even */
            val_out = (m & 2) ? -1 : 1;   /* (-1)^(m/2) */
        }                      /* else m odd → 0 already set */
    } else {          /* c even */
        if ((m & 1) == 0) {       /* m even */
            val_out = (m & 2) ? -1 : 1;   /* (-1)^(m/2) */
        } else {                    /* m odd */
            /* exponent = (m+1)/2 */
            /* parity of exponent is same as parity of (m+1)/2 */
            /* Since m odd, m=2k+1 → (m+1)/2 = k+1.
               If k even → exponent odd → -1; if k odd → exponent even → 1. */
            /* Instead of computing k, use parity of (m>>1)+1 */
            if ((((m >> 1) + 1) & 1) == 0)
                val_out = 1;
            else
                val_out = -1;
        }
    }

    return val_out;
}
