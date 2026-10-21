/*
 * A064100  a(n) = n * (100^n - 1)/99.
 * https://oeis.org/A064100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(100^n - 1)/99 Equivalent representation: a(n) = n *
 *            Σ_{k=0}^{n-1} 100^k (valid for n≥1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;             /* offset starts at 1, guard against 0 */
    seq_t rep = 0;
    seq_t p   = 1;
    for (idx_t i = 0; i < n; ++i) {
        rep += p;        /* add the current power of 100 to the sum */
        p   *= (seq_t)100; /* advance to the next power of 100 */
    }
    return (seq_t)n * rep;   /* multiply by n */
}
