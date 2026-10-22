/*
 * A006678  Number of planted binary phylogenetic trees with n labels.
 * https://oeis.org/A006678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. B(x) = 1 - sqrt(1-2x-2x^2) satisfies b(n+1) = (2n-1)*b(n)
 *            + (2n-4)*n*b(n-1), b(1) = 1, b(2) = 3; and since A(x) =
 *            B(x)/(1+x): a(n) = b(n) - n*a(n-1), a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    seq_t bprev = 1;   /* b(1) */
    seq_t bcur = 3;    /* b(2) */
    seq_t acur = 1;    /* a(2) */
    for (idx_t m = 2; m < n; m++) {
        seq_t bnext = (seq_t)(2 * m - 1) * bcur + (seq_t)(2 * m - 4) * (seq_t)m * bprev;
        seq_t anext = bnext - (seq_t)(m + 1) * acur;
        bprev = bcur;
        bcur = bnext;
        acur = anext;
    }
    return acur;
}
