/*
 * A024306  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n+1-k), where k=[
 *          (n+1)/2 ], s = (natural numbers >= 2), t = (natural numbers >= 3).
 * https://oeis.org/A024306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n^3 + 45*n^2 + 116*n + 48 - 3*(n+4)^2*(-1)^n) / 48
 * Range:     u8 none, u16 n <= 22, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t for all arithmetic to avoid overflow in intermediate steps */
    seq_t sn = (seq_t)n;
    seq_t n_sq = sn * sn;               /* n^2 */
    seq_t n_cu = n_sq * sn;             /* n^3 */
    seq_t n2p4 = sn + (seq_t)4;         /* n + 4 */
    seq_t n2p4_sq = n2p4 * n2p4;        /* (n+4)^2 */
    seq_t t = (seq_t)3 * n2p4_sq;       /* 3*(n+4)^2 */

    /* Base polynomial 4*n^3 + 45*n^2 + 116*n + 48 */
    seq_t base = (seq_t)4 * n_cu
               + (seq_t)45 * n_sq
               + (seq_t)116 * sn
               + (seq_t)48;

    seq_t numer;
    if ((n & (idx_t)1) == 0) {         /* n even: subtract t */
        numer = base - t;
    } else {                           /* n odd: add t */
        numer = base + t;
    }

    return numer / (seq_t)48;
}
