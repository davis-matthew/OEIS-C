/*
 * A155451  Triangle read by rows: T(n,m)=2*Binomial[n, m] - (m! - n! + (-m +
 *          n)!).
 * https://oeis.org/A155451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = triangle read by rows of T(n,m)=2*Binomial[n,m] - (m! -
 *            n! + (-m+n)!)
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the element a(n) for all n >= 0. */

#include "../utils/binomial.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2  */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        /* mid*(mid+1)/2 computed in seq_t to avoid overflow of idx_t */
        seq_t boundary = (seq_t)mid * ((seq_t)mid + 1) / 2;
        if (boundary <= (seq_t)n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* Column index m within that row */
    idx_t offset = r * (r + 1) / 2;
    idx_t m = n - offset;

    /* Compute the triangle value T(r,m) */
    seq_t bin   = binomial(r, m);          /* C(r,m) */
    seq_t two_c = bin + bin;               /* 2*Binomial(r,m) */
    seq_t fac_n = factorial(r);            /* n!   */
    seq_t fac_m = factorial(m);            /* m!   */
    seq_t fac_nm = factorial(r - m);       /* (n-m)! */

    /* T = 2*C + n! - m! - (n-m)!  (order chosen to avoid underflow) */
    seq_t result = two_c;
    result += fac_n;
    result -= fac_m;
    result -= fac_nm;

    return result;
}
