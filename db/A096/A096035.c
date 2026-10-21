/*
 * A096035  Triangle read by rows: T(n,k) = (n+1,k)-th element of (M^4-M)/3,
 *          where M is the infinite lower Pascal's triangle matrix, 1<=k<=n.
 * https://oeis.org/A096035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 121, u64 n <= 438,
 *            u128 n <= 1660, big any n
 * Helpers:   binomial, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = binomial(r, k-1) * (4^(r+1-k)-1)/3
 * where r = ceil((sqrt(1+8*n)-1)/2) is the row index,
 *       k = n - r*(r-1)/2    is the column index (1‑based).
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/pow4.h"       /* seq_t pow4(idx_t exp); */

static inline idx_t find_row(idx_t n)
{
    /* Find the smallest r such that r*(r+1)/2 >= n. */
    idx_t low = 1;
    idx_t high = n;            /* r never exceeds n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t sum = (__uint128_t)mid * (mid + 1) / 2;
        if (sum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* value outside OEIS offset, not used */

    /* Determine the (row, column) of the n‑th entry. */
    idx_t r = find_row(n);

    /* Number of elements before row r: 1 + 2 + ... + (r-1) = r*(r-1)/2 */
    __uint128_t prev = (__uint128_t)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev;          /* 1‑based column in row r */

    /* Compute the term using the explicit formula. */
    seq_t binom = binomial(r, k - 1);
    seq_t pow4exp = pow4((idx_t)(r + 1 - k));
    seq_t temp = pow4exp - 1;
    temp /= 3;                          /* exact division, 4^m ≡ 1 (mod 3) */

    return binom * temp;
}
