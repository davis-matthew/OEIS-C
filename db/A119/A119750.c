/*
 * A119750  Let k=binomial(n-1,2); a(n) = n*(n-1)*k!/(k-n+1)! for n >= 4, with
 *          a(1)=a(2)=a(3)=0.
 * https://oeis.org/A119750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)*k!/(k-n+1)! where k = binomial(n-1,2)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 3)
        return 0;

    /* Compute k = binomial(n-1,2) = (n-1)(n-2)/2 */
    unsigned __int128 tmp = (unsigned __int128)(n - 1) * (n - 2);
    idx_t k = (idx_t)(tmp / 2);

    /* Result starts with n*(n-1) */
    seq_t result = (seq_t)n * (seq_t)(n - 1);

    /* Multiply by the remaining factors from k-n+2 up to k */
    idx_t start = k - n + 2;
    for (idx_t i = start; i <= k; ++i)
    {
        result = result * (seq_t)i;
    }

    return result;
}
