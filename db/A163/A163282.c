/*
 * A163282  Triangle read by rows in which row n lists n+1 terms, starting
 *          with n^2 and ending with n^3, such that difference between
 *          successive terms is equal to n^2 - n.
 * https://oeis.org/A163282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = r^2 + k*(r^2 - r) where r = max m such that m*(m+1)/2 ≤ n
 *            and k = n - r*(r+1)/2, 0 ≤ k ≤ r
 * Range:     u8 n <= 32, u16 n <= 899, u32 n <= 1274, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t triangular(idx_t r)
{
    return r * (r + 1) / 2;
}

/* Find the row r such that triangular(r) ≤ n < triangular(r+1) */
static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 1;
    /* Increase high until it exceeds n */
    while (triangular(high) <= n)
    {
        /* Prevent overflow of high (unlikely for 64‑bit idx_t) */
        if (high > ((1ULL << 63) - 1) / 2) break;
        high <<= 1;
    }
    /* Binary search for the correct row */
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;   /* triangular(r) ≤ n < triangular(r+1) */
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    idx_t k = n - triangular(r);

    seq_t r_seq = (seq_t)r;
    seq_t r_sq  = r_seq * r_seq;
    seq_t diff  = r_sq - r_seq;   /* r^2 - r */
    seq_t k_seq = (seq_t)k;

    return r_sq + k_seq * diff;
}
