/*
 * A134559  Triangle read by rows: T(n,k) is the sum of the divisors of n that
 *          are >= k.
 * https://oeis.org/A134559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A134559.
   For n ≥ 1, let r be the smallest integer with r·(r+1)/2 ≥ n.
   Let k = n – (r–1)·r/2.  Then a(n) = Σ{d | r, d ≥ k} d. */

#include <stdint.h>

static inline idx_t cumulative(idx_t r)
{
    /* r*(r+1)/2 without overflow */
    if (r & 1)
        return r * ((r + 1) / 2);
    else
        return (r / 2) * (r + 1);
}

seq_t a(idx_t n)
{
    /* find the row index r */
    idx_t r = 0;
    while (cumulative(r) < n)
        ++r;

    /* column index k */
    idx_t prev = cumulative(r - 1);
    idx_t k = n - prev;

    /* sum of divisors of r that are >= k */
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= r; ++d)
    {
        if (r % d == 0)
        {
            if (d >= k)
                sum += (seq_t)d;
            idx_t e = r / d;
            if (e != d && e >= k)
                sum += (seq_t)e;
        }
    }
    return sum;
}
