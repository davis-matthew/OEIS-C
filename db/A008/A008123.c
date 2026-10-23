/*
 * A008123  Coordination sequence T1 for Zeolite Code KFI.
 * https://oeis.org/A008123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(7*m + k) = 84*m^2 + 24*k*m + c[k] + ((k==0 && n>0) ? 1 : 0)
 *            where c[k] = {1, 4, 9, 17, 29, 45, 64} for k = 0..6
 * Range:     u8 n <= 12, u16 n <= 195, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    idx_t m = n / 7;
    int k   = n % 7;

    /* coefficients for the constant term c[k] */
    static const seq_t c[7] = {1, 4, 9, 17, 29, 45, 64};

    seq_t m_s = (seq_t)m;             /* convert to seq_t to avoid overflow */
    seq_t k_s = (seq_t)k;

    seq_t base = (seq_t)84 * m_s * m_s + (seq_t)24 * m_s * k_s;
    seq_t result = base + c[k];

    /* for k==0 and n>0 the value is 1 higher than the base formula */
    if ((k == 0) && (n > 0))
        result += (seq_t)1;

    return result;
}
