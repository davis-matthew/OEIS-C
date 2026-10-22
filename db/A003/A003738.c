/*
 * A003738  Number of Hamiltonian paths in W_5 X P_n.
 * https://oeis.org/A003738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 60*a(n-1) - 128*a(n-2) - 3328*a(n-3) - 56832*a(n-4), a(1)
 *            = 24, a(2) = 1920, a(3) = 70184, a(4) = 2154592, a(5) = 58772296
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t a2 = (seq_t)1920, a3 = (seq_t)70184, a4 = (seq_t)2154592, a5 = (seq_t)58772296;
    idx_t i;

    if (n == 1) return (seq_t)24;
    if (n == 2) return a2;
    if (n == 3) return a3;
    if (n == 4) return a4;
    if (n == 5) return a5;

    /* sliding window: w1 = a(i-4), w2 = a(i-3), w3 = a(i-2), w4 = a(i-1) */
    {
        seq_t w1 = a2, w2 = a3, w3 = a4, w4 = a5;
        for (i = 6; i <= n; i++) {
            seq_t sub = w3 * 128 + w2 * 3328 + w1 * 56832;
            seq_t next = w4 * 60 - sub;
            w1 = w2;
            w2 = w3;
            w3 = w4;
            w4 = next;
        }
        return w4;
    }
}
