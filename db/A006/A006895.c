/*
 * A006895  Parenthesized one way gives the powers of 2: (1), (2), (1+3), ...,
 *          another way the powers of 3: (1), (2+1), (3+6), ....
 * https://oeis.org/A006895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(0) = 1, a(n) = u(n+1) - u(n), where u(0) = 0 < u(1) < u(2) <
 *            ... is the sorted union of {2^j - 1 : j >= 1} and {(3^k - 1)/2 :
 *            k >= 1} (the partial sums that hit the cumulative sums of the
 *            powers of 2 and of the powers of 3). The two sets share only the
 *            value 1
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 52, u64 n <= 103,
 *            u128 n <= 209, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    /* P = 2^j - 1, T = (3^k - 1)/2 are the next candidates of each kind */
    idx_t j = 2, k = 2;
    seq_t P = (seq_t)3, T = (seq_t)4;
    seq_t prev = (seq_t)0, cur = (seq_t)1;
    const double L3 = 1.5849625007211561814537389;  /* log2(3), used only on exponents */
    for (idx_t i = 1; i <= n; i++) {
        seq_t next;
        /* P < T  <=>  2^(j+1) < 3^k  <=>  j+1 < k*log2(3) (never equal here) */
        if ((double)(j + 1) < (double)k * L3) {
            next = P;
            P = P * 2 + 1;
            j++;
        } else {
            next = T;
            T = T * 3 + 1;
            k++;
        }
        prev = cur;
        cur = next;
    }
    return cur - prev;
}
