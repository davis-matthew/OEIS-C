/*
 * A211535  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w=4x+5y.
 * https://oeis.org/A211535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{y=1}^{⌊n/5⌋} ⌊(n-5y)/4⌋ This counts the number of
 *            pairs (x,y) with 1≤x,y≤n and 4x+5y≤n, which is equivalent to the
 *            definition of A211535
 * Range:     u8 n <= 105, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* For n<9 the sum is empty, giving a(n)=0 automatically. */
    seq_t total = 0;
    idx_t ymax = n / 5;          /* floor(n/5) */
    for (idx_t y = 1; y <= ymax; ++y) {
        /* n-5*y is non‑negative here; use seq_t for exact division. */
        seq_t add = (seq_t)(n - 5 * y) / 4;
        total += add;
    }
    return total;
}
