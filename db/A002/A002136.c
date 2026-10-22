/*
 * A002136  Matrices with 2 rows.
 * https://oeis.org/A002136
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002135(n-3) + (n-3)*A002135(n-4) + (n-3)*(n-4)*a(n-2),
 *            a(3) = 1, a(4) = 2, where A002135(m) = m*A002135(m-1) -
 *            ((m-1)*(m-2)/2)*A002135(m-3), A002135(0)=1, A002135(1)=1,
 *            A002135(2)=2. (OEIS's a(n+1) = A002135(n) + n*A002135(n-1) +
 *            n*(n-1)*a(n-1) uses the comment's indexing, shifted by 2.)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 3) return 0;
    idx_t K = n - 2;              /* c(K) with c(1)=1, c(2)=2 */
    if (K == 1) return 1;
    if (K == 2) return 2;

    /* q1 = P(k-3), q2 = P(k-2), q3 = P(k-1), where P = A002135 */
    seq_t q1 = 1, q2 = 1, q3 = 2;
    seq_t cm2 = 1, cm1 = 2;       /* c(k-2), c(k-1) */
    seq_t ck = 0;
    for (idx_t k = 3; k <= K; k++) {
        ck = q3 + (seq_t)(k - 1) * q2 + (seq_t)((k - 1) * (k - 2)) * cm2;
        cm2 = cm1;
        cm1 = ck;
        /* advance P: P(k) = k*P(k-1) - C(k-1,2)*P(k-3) */
        seq_t pk = (seq_t)k * q3 - (seq_t)((k - 1) * (k - 2) / 2) * q1;
        q1 = q2;
        q2 = q3;
        q3 = pk;
    }
    return ck;
}
