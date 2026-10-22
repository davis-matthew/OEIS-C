/*
 * A003898  Degrees of irreducible representations of group L3(8).
 * https://oeis.org/A003898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   degrees of irreducible characters of L3(q), q = 8, sorted
 *            (finite, 72 terms): 1 (x1), q(q+1) (x1), q^2+q+1 (x(q-2)),
 *            (q-1)^2(q+1) (x q(q+1)/3), (q-1)(q^2+q+1) (x q(q-1)/2), q^3
 *            (x1), q(q^2+q+1) (x(q-2)), (q+1)(q^2+q+1) (x(q-2)(q-3)/6)
 * Range:     u8 n <= 8, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const idx_t q = 8;
    idx_t cnt[8];
    seq_t deg[8];
    seq_t Q = (seq_t)q;
    int i;
    idx_t k;
    deg[0] = (seq_t)1;                        cnt[0] = 1;
    deg[1] = Q * (Q + 1);                     cnt[1] = 1;
    deg[2] = Q * Q + Q + 1;                   cnt[2] = q - 2;
    deg[3] = (Q - 1) * (Q - 1) * (Q + 1);     cnt[3] = q * (q + 1) / 3;
    deg[4] = (Q - 1) * (Q * Q + Q + 1);       cnt[4] = q * (q - 1) / 2;
    deg[5] = Q * Q * Q;                       cnt[5] = 1;
    deg[6] = Q * (Q * Q + Q + 1);             cnt[6] = q - 2;
    deg[7] = (Q + 1) * (Q * Q + Q + 1);       cnt[7] = (q - 2) * (q - 3) / 6;
    k = n;
    for (i = 0; i < 8; i++) {
        if (k <= cnt[i]) return deg[i];
        k -= cnt[i];
    }
    return (seq_t)0;
}
