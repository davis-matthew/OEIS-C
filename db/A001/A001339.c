/*
 * A001339  a(n) = Sum_{k=0..n} (k+1)! binomial(n,k).
 * https://oeis.org/A001339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+2)*a(n-1) - (n-1)*a(n-2), a(0) = 1, a(1) = 3 (from the
 *            E.g.f. exp(x)/(1-x)^2, which satisfies (1-x)*A' = (3-x)*A)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prev, cur, next;
    idx_t i;
    if (n == 0) return 1;
    prev = 1;
    cur = 3;
    for (i = 2; i <= n; i++) {
        next = ((seq_t)i + 2) * cur - ((seq_t)i - 1) * prev;
        prev = cur;
        cur = next;
    }
    return cur;
}
