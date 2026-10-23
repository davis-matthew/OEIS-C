/*
 * A162319  Array read by antidiagonals: a(n,m) = the number of digits of m is
 *          when written in base n. The top row is the number of digits for
 *          each m in base 1.
 * https://oeis.org/A162319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, tabl
 * Formula:   a(n) = number of digits of m in base b, where (b,m) are
 *            determined by index n via reading the array a(b,m) (number of
 *            digits of m in base b) by antidiagonals with (b,m) ordered as:
 *            for each d≥2, n runs from d-1 down to 1 and m = d - n
 * Range:     u8 n <= 9123, u16 n <= 9123, u32 n <= 9123, u64 n <= 9123,
 *            u128 n <= 9123, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t t)
{
    /* Find smallest d with d*(d-1)/2 >= t.
       Use binary search with 128‑bit arithmetic to avoid overflow. */
    uint64_t low = 1, high = 1;
    while ( (__uint128_t)high * (high - 1) / 2 < t )
        high <<= 1;
    while (low < high) {
        uint64_t mid = low + ((high - low) >> 1);
        __uint128_t c = (__uint128_t)mid * (mid - 1) / 2;
        if (c >= t)
            high = mid;
        else
            low = mid + 1;
    }
    uint64_t d = low;                      /* diagonal sum n+m = d */
    __uint128_t prev_c = (__uint128_t)(d - 1) * (d - 2) / 2; /* elements before this diagonal */
    uint64_t offset = (uint64_t)(t - prev_c - 1);            /* 0‑based offset inside diagonal */

    uint64_t base  = d - 1 - offset;                      /* n */
    uint64_t m_val = offset + 1;                            /* m */

    /* Compute number of digits of m_val in base 'base'. */
    seq_t len = 0;
    if (base == 1) {                  /* unary base: length equals m */
        len = (seq_t)m_val;
    } else {
        seq_t mm = (seq_t)m_val;
        seq_t b  = (seq_t)base;
        while (mm) {
            ++len;
            mm /= b;
        }
    }
    return len;
}
