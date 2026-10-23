/*
 * A131240  T(n,k) = 2*A046854(n,k) - I.
 * https://oeis.org/A131240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 360, u32 n <= 1245, u64 n <= 4695,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A131240: a(n) is the n-th element of the triangle
   T(n,k) = 2*A046854(n,k) - Identity matrix
   where A046854(n,k) = binomial(floor((n+k)/2), k)
   The sequence is the triangle enumerated row by row. */

#include <stdint.h>
/* Use the utility functions if needed; none required here */

/* small 64‑bit GCD, used only for reduction in 64‑bit builds */
static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* binomial coefficient for small n (≤ 10000); works for all seq_t */
static seq_t my_binomial(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;

    seq_t result = 1;

    for (idx_t i = 1; i <= k; ++i) {
        uint64_t num = n - k + i;   /* numerator (≤ 10000) */
        uint64_t den = i;           /* denominator (≤ 10000) */

        /* cancel common factors between num and den */
        uint64_t g = gcd_uint64(num, den);
        num /= g;
        den /= g;

        /* for 64‑bit seq_t use 128‑bit intermediate to avoid overflow */
        if (sizeof(seq_t) <= 8) {
            uint64_t res64 = (uint64_t)result;

            /* cancel factors of den from current result */
            uint64_t g2 = gcd_uint64(res64, den);
            res64 /= g2;
            den /= g2;
            result = (seq_t)res64;

            unsigned __int128 temp = (unsigned __int128)result * num;
            if (den != 1) temp /= den;
            result = (seq_t)temp;
        } else { /* arbitrary‑precision seq_t */
            /* division by den is exact for binomial */
            result = result * (seq_t)num;
            if (den != 1) result /= (seq_t)den;
        }
    }
    return result;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = (idx_t) 0xFFFFFFFFULL;  /* safe upper bound */
    idx_t r = 0;

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2u;
        if (t <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    unsigned __int128 tri_before = (unsigned __int128)r * (r + 1) / 2u;
    idx_t col = n - (idx_t)tri_before;                /* column index in row r */
    idx_t m   = (r + col) >> 1u;                      /* floor((r+col)/2) */

    seq_t binom = my_binomial(m, col);                /* A046854(r, col) */

    /* compute T = 2*binom - (col==r?1:0) */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 temp = (unsigned __int128)binom * 2u;
        if (col == r) temp -= 1;
        return (seq_t)temp;
    } else {
        seq_t t = binom + binom;
        if (col == r) t -= 1;
        return t;
    }
}
