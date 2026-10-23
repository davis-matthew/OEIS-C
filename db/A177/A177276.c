/*
 * A177276  Triangle T(n,k) with the coefficient [x^k] of the polynomial
 *          (1+x^2)^n + 2*n*x*(1+x^2)^(n-1) in row n, column k, 0<=k<=2n.
 * https://oeis.org/A177276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = coefficient of x^(n – r²) in (1+x²)^r +
 *            2*r*x*(1+x²)^(r‑1) where r = floor(sqrt(n)). The coefficient is
 *            * binom(r, k/2) if k is even, * 2*r*binom(r‑1, (k‑1)/2) if k is
 *            odd, with k = n – r². The triangle T(n,k) is flattened row by
 *            row to form the sequence a(n)
 * Range:     u8 n <= 55, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE:  This function uses only the integer types defined in the
   surrounding build (idx_t and seq_t) and the utility
   function binom(idx_t, idx_t).  It does not rely on any floating‑point
   arithmetic or external libraries. */

#include "../utils/binom.h"

/* Integer square‑root (floor of √n) using binary search.  This is
   portable to both C11 and C++17. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low  = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid > 0 && mid > n / mid)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Determine row r and position k in that row. */
    idx_t r = isqrt_u64(n);          /* r² ≤ n < (r+1)² */
    idx_t k = n - r * r;             /* 0 ≤ k ≤ 2r */

    if ((k & 1U) == 0) {            /* even k → only the (1+x²)^r part */
        idx_t m = k >> 1;           /* m = k/2 */
        return binom(r, m);
    } else {                          /* odd k → the 2*r*x*(1+x²)^(r-1) part */
        /* For r = 0 we cannot have an odd k, so this block is safe. */
        idx_t m = (k - 1) >> 1;     /* m = (k-1)/2 */
        seq_t c = binom(r - 1, m);
        seq_t mult = (seq_t)(2 * r); /* 2*r fits into idx_t for all n ≤ 2^64-1 */
        return c * mult;
    }
}
