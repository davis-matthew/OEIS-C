/*
 * A001342  E.g.f.: 24*exp(x)/(1-x)^5.
 * https://oeis.org/A001342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = (n+6)*a(n) - n*a(n-1), a(0) = 24, a(1) = 144 (from
 *            E.g.f. 24*exp(x)/(1-x)^5, equivalently a(n) = Sum_{k=0..n}
 *            binomial(n,k)*(k+4)!)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prev = 0;
    seq_t cur = 24;
    idx_t i;
    for (i = 0; i < n; i++) {
        seq_t next = (seq_t)(i + 6) * cur;
        next -= (seq_t)i * prev;
        prev = cur;
        cur = next;
    }
    return cur;
}
