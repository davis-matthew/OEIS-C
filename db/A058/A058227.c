/*
 * A058227  Number of edges in all simple (loopless) paths, connecting any
 *          node with all the remaining ones in optimal graphs of degree 4.
 * https://oeis.org/A058227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 4 * Σ_{k=1..n} k * (2^k - 1)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(0)=0, a(n)=a(n-1)+4*n*(2^n-1)  (n≥1) */

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* not part of the OEIS offset, harmless      */
    seq_t result = 0;
    seq_t power  = 1;                /* 2^0 */
    for (idx_t k = 1; k <= n; ++k) {
        power <<= 1;                 /* multiply by 2  → 2^k */
        seq_t term = (seq_t)k * (power - 1);
        result += term << 2;         /* multiply by 4  */
    }
    return result;
}
