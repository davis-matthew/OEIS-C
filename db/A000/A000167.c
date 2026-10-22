/*
 * A000167  Nearest integer to modified Bessel function K_n(2).
 * https://oeis.org/A000167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(K_n(2)). At x = 2 the log term of the K_n series is
 *            zero, so 2*K_n(2) = I + F + (tiny), where I =
 *            Sum_{k=0}^{ceil(n/2)-1} (-1)^k (n-k-1)!/k! (an integer) and F =
 *            Sum_{k>=ceil(n/2)}^{n-1} (-1)^k (n-k-1)!/k! (0 < |F| < 1, sign
 *            (-1)^ceil(n/2)). Hence a(n) = I/2 if I is even, else (I + (...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    idx_t k0 = (n + 1) / 2;
    seq_t q;
    idx_t p;
    if (n % 2 == 0) {            /* t_{k0-1} = k0 */
        q = (seq_t)(k0 / 2);
        p = k0 & 1;
    } else {                     /* t_{k0-1} = 1 */
        q = 0;
        p = 1;
    }
    seq_t qp = 0, qn = 0;
    long long R = 0;
    idx_t k = k0 - 1;
    for (;;) {
        if (k % 2 == 0) {
            qp += q;
            R += (long long)p;
        } else {
            qn += q;
            R -= (long long)p;
        }
        if (k == 0)
            break;
        k--;
        idx_t m = (k + 1) * (n - k - 1);
        idx_t pm = p ? m : 0;
        q = q * m + (seq_t)(pm / 2);
        p = pm & 1;
    }
    seq_t Q = qp - qn;           /* I = 2*Q + R */
    long long c = R;
    if (R % 2 != 0)
        c += (k0 % 2 == 0) ? 1 : -1;
    if (c >= 0)
        return Q + (seq_t)(c / 2);
    return Q - (seq_t)((-c) / 2);
}
