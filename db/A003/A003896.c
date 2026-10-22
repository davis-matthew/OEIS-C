/*
 * A003896  Degrees of irreducible representations of group L3(5).
 * https://oeis.org/A003896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   character degrees of L3(q)=SL(3,q), q=5 (gcd(3,q-1)=1), sorted:
 *            1 (x1), q^2+q (x1), q^2+q+1 (x(q-2)), (q-1)^2(q+1) (x(q^2+q)/3),
 *            (q-1)(q^2+q+1) (x(q^2-q)/2), q^3 (x1), q(q^2+q+1) (x(q-2)),
 *            (q+1)(q^2+q+1) (x(q-2)(q-3)/6); finite sequence of 30 terms
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const unsigned long long q = 5;
    seq_t Q = (seq_t)q;
    seq_t deg[8];
    unsigned long long cnt[8];
    int i;
    idx_t k = n;

    deg[0] = (seq_t)1;                               cnt[0] = 1;
    deg[1] = Q * Q + Q;                              cnt[1] = 1;
    deg[2] = Q * Q + Q + 1;                          cnt[2] = q - 2;
    deg[3] = (Q - 1) * (Q - 1) * (Q + 1);            cnt[3] = (q * q + q) / 3;
    deg[4] = (Q - 1) * (Q * Q + Q + 1);              cnt[4] = (q * q - q) / 2;
    deg[5] = Q * Q * Q;                              cnt[5] = 1;
    deg[6] = Q * (Q * Q + Q + 1);                    cnt[6] = q - 2;
    deg[7] = (Q + 1) * (Q * Q + Q + 1);              cnt[7] = (q - 2) * (q - 3) / 6;

    if (k < 1) return (seq_t)0;
    for (i = 0; i < 8; i++) {
        if (k <= cnt[i]) return deg[i];
        k -= cnt[i];
    }
    return (seq_t)0;
}
