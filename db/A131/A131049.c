/*
 * A131049  Triangle read by rows: (1/4) * (A007318^3 - A007318^(-1)) as
 *          infinite lower triangular matrices.
 * https://oeis.org/A131049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{k=0}^{r-1} binomial(r,k)*(3^{r-k} - (-1)^{r-k})/4, *
 *            where r is the smallest integer with r(r+1)/2 >= n. The function
 *            * returns the n‑th term of OEIS sequence A131049, using the *
 *            triangular construction above
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 173, u64 n <= 601,
 *            u128 n <= 1275, big any n
 * Helpers:   binomial, isqrt, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt.h"   /* _floor_isqrt_u64 if needed */
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* OEIS offset starts at 1 */
    if (n == 0) return 0;

    /* Find smallest r such that r(r+1)/2 >= n.
       Binary search with 128‑bit arithmetic to avoid overflow. */
    idx_t lo = 0, hi = n, r = 0;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __uint128_t cum = (__uint128_t)mid * (mid + 1) / 2;
        if (cum >= n) {
            r  = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }

    /* Sum of elements preceding row r */
    __uint128_t prev_sum128 = (__uint128_t)(r - 1) * r / 2;
    idx_t prev_sum = (idx_t)prev_sum128;   /* fits into idx_t */

    /* Column k (0‑based) within row r */
    idx_t k = n - prev_sum - 1;
    /* Exponent: e = r - k */
    idx_t e = r - k;

    /* Compute binomial coefficient */
    seq_t bin = binomial(r, k);

    /* Compute 3^e as seq_t */
    seq_t pow3e = pow_uint(3, e);

    /* Compute 3^e - (-1)^e, avoiding negative values */
    seq_t diff = (e & 1) ? (pow3e + 1) : (pow3e - 1);

    /* Division by 4 (exact by construction) */
    seq_t diff_div4 = diff / 4;

    /* Final result */
    return bin * diff_div4;
}
