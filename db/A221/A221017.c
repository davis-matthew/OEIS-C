/*
 * A221017  The hyper-Wiener index of the fullerenyl anion G[n]C[60], defined
 *          pictorially in the Arezoomand reference (see Fig. 5).
 * https://oeis.org/A221017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14198+2^n*(-11544+6920*n+14000*n^2)
 *            +4^n*(6400*n^2-25600*n+45096)
 * Range:     u8 none, u16 n <= 0, u32 n <= 7, u64 n <= 21, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)47750;
    }
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;

    seq_t pow2 = (seq_t)1;
    seq_t pow4 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 = pow2 * (seq_t)2;
        pow4 = pow4 * (seq_t)4;
    }

    seq_t t1 = (seq_t)6920 * n_seq + (seq_t)14000 * n2 - (seq_t)11544;
    seq_t t2 = (seq_t)6400 * n2 - (seq_t)25600 * n_seq + (seq_t)45096;

    seq_t result = (seq_t)14198 + pow2 * t1 + pow4 * t2;
    return result;
}
