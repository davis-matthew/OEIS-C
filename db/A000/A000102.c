/*
 * A000102  a(n) = number of compositions of n in which the maximum part size
 *          is 4.
 * https://oeis.org/A000102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = C4(n) - C3(n), where Ck(n) = number of compositions of n
 *            into parts <= k: Ck(0) = 1, Ck(m) = Ck(m-1) + ... + Ck(m-k)
 *            (Ck(<0) = 0). Equivalently G.f.:
 *            x^4/((1-x-x^2-x^3)(1-x-x^2-x^3-x^4))
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 68, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* q[0..3] = C4(m-3..m), t[0..2] = C3(m-2..m) */
    seq_t q0 = 0, q1 = 0, q2 = 0, q3 = 1;
    seq_t t0 = 0, t1 = 0, t2 = 1;
    idx_t m;
    for (m = 1; m <= n; m++) {
        seq_t nq = q0 + q1 + q2 + q3;
        seq_t nt = t0 + t1 + t2;
        q0 = q1; q1 = q2; q2 = q3; q3 = nq;
        t0 = t1; t1 = t2; t2 = nt;
    }
    return q3 - t2;
}
