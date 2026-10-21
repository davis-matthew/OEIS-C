/*
 * A202442  Number of (n+2) X 5 binary arrays avoiding patterns 001 and 110 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 224 if n is even: a(n) = (n^4 + 24 n^3 + 149 n^2 + 492 n
 *            + 468) / 6 if n is odd > 1: a(n) = (n^4 + 24 n^3 + 149 n^2 + 498
 *            n + 492) / 6 (The polynomials are integer‑valued for all n ≥ 2.)
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)224;

    /* Cast n to seq_t once and precompute powers */
    seq_t sn  = (seq_t)n;
    seq_t n2  = sn * sn;          /* n^2 */
    seq_t n3  = n2 * sn;          /* n^3 */
    seq_t n4  = n2 * n2;          /* n^4 */

    seq_t num;
    if ((sn & (seq_t)1) == 0) {   /* n even */
        num = n4
            + (seq_t)24 * n3
            + (seq_t)149 * n2
            + (seq_t)492 * sn
            + (seq_t)468;
    } else {                     /* n odd > 1 */
        num = n4
            + (seq_t)24 * n3
            + (seq_t)149 * n2
            + (seq_t)498 * sn
            + (seq_t)492;
    }

    return num / (seq_t)6;
}
