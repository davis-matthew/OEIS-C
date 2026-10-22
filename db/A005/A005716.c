/*
 * A005716  Coefficient of x^8 in expansion of (1+x+x^2)^n.
 * https://oeis.org/A005716
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A027907(n, 8), computed by the trinomial recurrence
 *            T(m,k) = T(m-1,k) + T(m-1,k-1) + T(m-1,k-2), T(0,0) = 1, T(0,k)
 *            = 0 for k > 0; a(n) = T(n,8)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 57, u64 n <= 960,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t t[9];
    int k;
    idx_t m;
    for (k = 0; k < 9; k++)
        t[k] = 0;
    t[0] = 1;
    for (m = 0; m < n; m++) {
        for (k = 8; k >= 0; k--) {
            seq_t s = t[k];
            if (k >= 1)
                s += t[k - 1];
            if (k >= 2)
                s += t[k - 2];
            t[k] = s;
        }
    }
    return t[8];
}
