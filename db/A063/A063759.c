/*
 * A063759  Spherical growth series for modular group.
 * https://oeis.org/A063759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=4, a(n)=2*a(n-2) for n>=3
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 61, u64 n <= 125,
 *            u128 n <= 253, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* In closed form: a(2k)=2^{k+1} (k>=1), a(2k+1)=3*2^{k} (k>=0) */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if (n & 1)          /* odd n : a(2k+1) = 3*2^k, k = (n-1)/2 */
        return ((seq_t)3) << ((n - 1) / 2);
    else                /* even n>0 : a(2k) = 2^{k+1}, k = n/2 */
        return ((seq_t)1) << ((n / 2) + 1);
}
