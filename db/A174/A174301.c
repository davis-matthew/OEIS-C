/*
 * A174301  A symmetrical triangle: T(n,k) = binomial(n, k)*if(floor(n/2)
 *          greater than or equal to k then 4^k, otherwise 4^(n-k)).
 * https://oeis.org/A174301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r,k) * 4^( (floor(r/2) >= k) ? k : r-k ), where
 *            r is the unique row index satisfying r(r+1)/2 ≤ n < (r+1)(r+2)/2
 *            and k = n – r(r+1)/2
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 179, u64 n <= 611,
 *            u128 n <= 2243, big any n
 * Helpers:   binomial, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow4.h"

#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 u128;
#endif

/* Find the row index r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
static idx_t _row_from_index(idx_t n)
{
#if defined(__SIZEOF_INT128__)
    idx_t lo = 0, hi = n + 1;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        u128 prod = (u128)mid * (mid + 1);          /* mid*(mid+1) */
        /* prod/2 ≤ n  ⇔  prod ≤ 2*n  */
        if (prod <= (u128)n * 2)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
#else
    /* Linear scan – fast enough for expected n (≈10⁴). */
    idx_t r = 0;
    while ((u128)(r + 1) * (r + 2) / 2 <= n)
        ++r;
    return r;
#endif
}

seq_t a(idx_t n)
{
    idx_t r   = _row_from_index(n);

    /* offset of the first element in row r */
    idx_t offset = (idx_t)(((u128)r * (r + 1)) / 2);
    idx_t k      = n - offset;                      /* column index */

    idx_t half = r >> 1;                            /* floor(r/2) */
    idx_t exp  = (half >= k) ? k : (r - k);

    seq_t binom = binomial(r, k);
    seq_t pow4v = pow4(exp);

    return binom * pow4v;
}
