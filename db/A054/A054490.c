/*
 * A054490  Expansion of (1+5*x)/(1-6*x+x^2).
 * https://oeis.org/A054490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2), a(0)=1, a(1)=11
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)11;
    seq_t prev = (seq_t)1;
    seq_t curr = (seq_t)11;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = (seq_t)((seq_t)6 * curr - prev);
        prev = curr;
        curr = next;
    }
    return curr;
}
