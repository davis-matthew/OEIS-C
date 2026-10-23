/*
 * A130595  Triangle read by rows: lower triangular matrix which is inverse to
 *          Pascal's triangle (A007318) regarded as a lower triangular matrix.
 * https://oeis.org/A130595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, nice, tabl
 * Formula:   not stated
 * Range:     i8 n <= 58, i16 n <= 178, i32 n <= 610, i64 n <= 2307,
 *            i128 n <= 7874, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (-1)^(r-k) * C(r,k)  where r and k are chosen so
 *                r*(r+1)/2 <= n < (r+1)*(r+2)/2  and k = n - r*(r+1)/2.
 * C(r,k) is the binomial coefficient, computed multiplicatively.
 */
#include <stdint.h>  /* needed for int64_t etc. but seq_t/idx_t are supplied */

/* local multiplicative implementation of binomial(r,k) */
static seq_t binomial_value(idx_t n, idx_t k)
{
    if (k > n) return 0;
    /* use symmetry */
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* use 128‑bit intermediate to avoid overflow of the product */
        unsigned __int128 temp = (unsigned __int128)result * (n - k + i);
        result = (seq_t)(temp / i);
    }
    return result;
}

seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);  /* upper mid to avoid infinite loop */
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n) low = mid;
        else          high = mid - 1;
    }
    idx_t r = low;

    /* column index k */
    unsigned __int128 tri_r = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)tri_r;

    seq_t bin = binomial_value(r, k);

    /* sign: (-1)^(r-k) */
    if (((r - k) & 1) != 0) return -bin;
    return bin;
}
