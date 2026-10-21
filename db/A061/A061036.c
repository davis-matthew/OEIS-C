/*
 * A061036  Triangle T(m,n) = denominator of 1/m^2 - 1/n^2, n >= 1,
 *          m=n,n-1,n-2,...,1.
 * https://oeis.org/A061036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, nice, frac
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 137, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) as the denominator of 1/m² − 1/n² for the pair (m,n) that
   corresponds to the n‑th element of the triangle
   T(m,n) = denom(1/m² − 1/n²), listing rows with m descending. */
#include <stdint.h>
#include "../utils/gcd.h"

static inline idx_t find_row(idx_t N)
{
    idx_t low = 1, high = N;
    /* Find the smallest row r such that r*(r+1)/2 >= N. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t t = (__uint128_t)mid * (mid + 1) / 2;
        if (t >= N)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t N)
{
    /* N is the 1‑based index in the sequence.  The triangle is indexed by
       rows n = 1,2,…, each row containing n terms for m = n,n−1,…,1. */
    idx_t row = find_row(N);

    /* Total number of terms before this row: T(row‑1) = (row‑1)*row/2. */
    __uint128_t prev_tr = (__uint128_t)(row - 1) * row / 2;
    idx_t j = N - (idx_t)prev_tr;           /* 1‑based offset within the row. */
    idx_t m = row - j + 1;                  /* m runs from row down to 1. */

    seq_t m_seq = (seq_t)m;
    seq_t n_seq = (seq_t)row;

    seq_t m2 = m_seq * m_seq;
    seq_t n2 = n_seq * n_seq;
    seq_t num = (n2 >= m2) ? (n2 - m2) : (m2 - n2);  /* |n²−m²|, non‑negative. */

    seq_t g1 = gcd(num, m2);
    seq_t num1 = num / g1;
    seq_t m2_div = m2 / g1;
    seq_t g2 = gcd(num1, n2);

    seq_t denom = m2_div * (n2 / g2);
    return denom;
}
