/*
 * A001243  Eulerian numbers (Euler's triangle: column k=7 of A008292, column
 *          k=6 of A173018).
 * https://oeis.org/A001243
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(n,7) where T(n,k) = k*T(n-1,k) + (n-k+1)*T(n-1,k-1),
 *            T(1,1) = 1, T(1,k) = 0 for k > 1 (Eulerian numbers A008292)
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 13, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t E[8];
    int k;
    idx_t m;

    for (k = 0; k < 8; k++)
        E[k] = 0;
    E[1] = 1;

    for (m = 2; m <= n; m++) {
        for (k = 7; k >= 2; k--) {
            seq_t t = E[k] * (seq_t)k;
            if (m + 1 >= (idx_t)k)
                t += E[k - 1] * (seq_t)(m + 1 - (idx_t)k);
            E[k] = t;
        }
        E[1] = 1;
    }

    return E[7];
}
