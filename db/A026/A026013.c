/*
 * A026013  a(n) = number of (s(0), s(1), ..., s(2n)) such that s(i) is a
 *          nonnegative integer and |s(i) - s(i-1)| = 1 for i = 1,2,...,n,
 *          s(0) = 2, s(2n) = 4. Also a(n) = T(2n,n-1), where T is the array
 *          defined in A026009.
 * https://oeis.org/A026013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n, n-1) - binomial(2*n, n-4)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiplicative binomial coefficient.  It works correctly for any
   seq_t, whether a machine integer type or an arbitrary‑precision
   integer class.  For 64‑bit seq_t it uses unsigned __int128 to avoid
   overflow of intermediate products. */
static seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        idx_t addend = n - k + i; /* the numerator term */

        /* For small integer seq_t types (≤ 64 bits) use __int128 to
           keep the intermediate product from overflowing.  For
           arbitrary‑precision seq_t (larger than 64 bits) we rely on
           the class's own multiplication and division. */
        if (sizeof(seq_t) <= 8)
        {
            unsigned __int128 temp = (unsigned __int128)result * (unsigned __int128)addend;
            temp /= (unsigned __int128)i;
            result = (seq_t)temp;
        }
        else
        {
            result = result * (seq_t)addend;
            result = result / (seq_t)i;
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* sequence starts at n = 1, but handle 0 safely */
        return 0;

    idx_t two_n = n * 2;
    seq_t term1 = binom_calc(two_n, n - 1);

    seq_t term2 = 0;
    if (n >= 4)
        term2 = binom_calc(two_n, n - 4);

    return term1 - term2;
}
