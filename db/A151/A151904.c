/*
 * A151904  a(n) = (3^(wt(k)+f(j))-1)/2 if n = 6k+j, 0 <= j < 6, where wt =
 *          A000120, f = A151899.
 * https://oeis.org/A151904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^(wt(k)+f(j)) - 1)/2, where n = 6k + j, 0 <= j < 6, wt
 *            = binary popcount, f(j) = [0,0,1,1,1,2]
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* f(j) values for j = 0..5 */
    static const idx_t f_arr[6] = {0, 0, 1, 1, 1, 2};

    idx_t k = n / 6;
    idx_t j = n % 6;

    /* popcount of k (weight of binary digits) */
    idx_t wt = 0;
    idx_t tmp = k;
    while (tmp)
    {
        wt += tmp & 1;
        tmp >>= 1;
    }

    idx_t p = wt + f_arr[j];

    /* compute 3^p */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < p; ++i)
        pow3 *= 3;

    return (pow3 - 1) / 2;
}
