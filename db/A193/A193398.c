/*
 * A193398  Hyper-Wiener index of a benzenoid consisting of a double-step
 *          spiral chain of n hexagons (n >= 2, s = 21; see the Gutman et al.
 *          reference).
 * https://oeis.org/A193398
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n^4 + 48*n^3 + 146*n^2 - 316*n + 439 + (-1)^n*(6*n^2 +
 *            24*n - 83))/4
 * Range:     u8 none, u16 n <= 8, u32 n <= 228, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n2    = n_seq * n_seq;
    seq_t n3    = n2 * n_seq;
    seq_t n4    = n3 * n_seq;

    seq_t term1 = 6 * n4;
    seq_t term2 = 48 * n3;
    seq_t term3 = 146 * n2;
    seq_t base  = term1 + term2 + term3 + 439;          /* 6n^4 + 48n^3 + 146n^2 + 439 */
    seq_t sub316 = 316 * n_seq;                         /* 316n */

    seq_t inner = 6 * n2 + 24 * n_seq - 83;             /* 6n^2 + 24n - 83 */

    seq_t termA;
    if (n_seq & 1) {   /* n odd: subtract inner */
        termA = base - sub316 - inner;
    } else {           /* n even: add inner */
        termA = base + inner - sub316;
    }

    return termA / 4;
}
