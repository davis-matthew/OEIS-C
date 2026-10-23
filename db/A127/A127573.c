/*
 * A127573  Triangle T(n, k) = k*sigma(k) if k divides n, else 0.
 * https://oeis.org/A127573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where * r is the row with triangular(r-1) < n ≤
 *            triangular(r), * k = n - triangular(r-1); * T(r,k) = k *
 *            sigma(k) if k divides r, otherwise 0
 * Range:     u8 n <= 77, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


/* Compute the sum of positive divisors of k (sigma(k)). */
static seq_t sigma_of(idx_t k)
{
    seq_t sum = 0;
    for (idx_t d = 1; (unsigned __int128)d * d <= k; ++d)
    {
        if (k % d == 0)
        {
            sum += (seq_t)d;
            idx_t q = k / d;
            if (q != d)
                sum += (seq_t)q;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, ignore index 0 */
        return 0;

    /* Find the row r such that r*(r+1)/2 ≥ n */
    idx_t r = 1;
    while ( ((seq_t)r * (r + 1) / 2) < n )
        ++r;

    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;           /* column within row r */

    if (r % k != 0)
        return 0;                /* entry is zero */

    /* T(r,k) = k * sigma(k) */
    seq_t sigma = sigma_of(k);
    return (seq_t)k * sigma;
}
