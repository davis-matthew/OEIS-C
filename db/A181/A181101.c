/*
 * A181101  Triangle T(n,k) read by rows. T(1,1)=-1, n>1: If n/k=A020639(n)
 *          then 1 else 0.
 * https://oeis.org/A181101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 105, i16 n <= 105, i32 n <= 105, i64 n <= 105,
 *            i128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = -1; for n>1 let r be the row such that r(r-1)/2 < n ≤ r(r+1)/2.
   Let k = n – r(r-1)/2.  Then a(n) = 1 iff k = r / lpf(r),
   where lpf(r) is the least prime factor of r. */
#include <stdint.h>

static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = (n >> 1) + 1;
    while (low <= high)
    {
        idx_t mid = (low + high) >> 1;
        if (mid > 0 && mid > n / mid)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return low - 1;
}

static idx_t least_prime_factor(idx_t n)
{
    if (n <= 1)
        return n;
    idx_t out = n;
    for (idx_t d = 2; d * d <= n; ++d)
        if (n % d == 0)
        {
            out = d;
            break;
        }
    return out;
}

seq_t a(idx_t N)
{
    if (N == 1)
        return (seq_t)-1;

    /* find row r */
    idx_t m = N - 1;
    idx_t t = 8 * m + 1;
    idx_t s = isqrt_u64(t);
    idx_t r0 = (s - 1) / 2;
    idx_t r  = r0 + 1;

    /* column index k */
    idx_t tri_prev = r0 * (r0 + 1) / 2;
    idx_t k = N - tri_prev;

    /* least prime factor of r */
    idx_t lpf = least_prime_factor(r);

    idx_t target_k = r / lpf;

    return (k == target_k) ? (seq_t)1 : (seq_t)0;
}
