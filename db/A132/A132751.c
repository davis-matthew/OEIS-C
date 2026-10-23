/*
 * A132751  Triangle T(n, k) = 2/Beta(n-k+1, k) - 1, read by rows.
 * https://oeis.org/A132751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) with r = smallest integer such that r(r+1)/2 >= n,
 *            k = n - r(r-1)/2, and T(r,k) = 2*(r-k+1)*C(r,k-1) - 1. C(r,k-1)
 *            is the binomial coefficient binomial(r,k-1). The triangular
 *            numbers r(r+1)/2 give the cumulative counts of terms
 * Range:     u8 n <= 24, u16 n <= 111, u32 n <= 449, u64 n <= 1918,
 *            u128 n <= 5050, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A132751: triangle read by rows of values
   T(n,k) = 2/Beta(n-k+1,k) - 1 = 2*(n-k+1)*C(n,k-1) - 1        */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t low = 1;
    idx_t high = n;          /* row r never exceeds n due to r(r+1)/2 >= n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Compute k = index within row r */
    unsigned __int128 cum_before = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = (idx_t)(n - cum_before);   /* 1 <= k <= r */

    /* Compute T(r,k) = 2*(r-k+1)*C(r,k-1) - 1 */
    idx_t nk1 = r - k + 1;
    seq_t bin = binomial(r, k - 1);
    seq_t product = bin * (seq_t)nk1;   /* (r-k+1)*C(r,k-1) */
    seq_t result = product * (seq_t)2;  /* 2*(r-k+1)*C(r,k-1) */
    result -= (seq_t)1;                /* -1 */

    return result;
}
