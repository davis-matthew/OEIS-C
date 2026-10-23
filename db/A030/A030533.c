/*
 * A030533  Expansion of Molien series for 4-D extraspecial group 2^{1+2*2}.
 * https://oeis.org/A030533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*k^3+3*k^2+7*k+3)/3 if n even (k = n/2) a(n) =
 *            (2*k^3+6*k^2+7*k+3)/3 if n odd (k = (n-1)/2)
 * Range:     u8 n <= 9, u16 n <= 90, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t k, k2, k3, num, res;

    if ((n & 1) == 0) {                /* n is even */
        k  = (seq_t)(n / 2);
        k2 = k * k;
        k3 = k2 * k;
        num = 2 * k3 + 3 * k2 + 7 * k + 3;
    } else {                            /* n is odd */
        k  = (seq_t)((n - 1) / 2);
        k2 = k * k;
        k3 = k2 * k;
        num = 2 * k3 + 6 * k2 + 7 * k + 3;
    }

    res = num / 3;                      /* integer division is exact */
    return res;
}
