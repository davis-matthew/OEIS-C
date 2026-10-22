/*
 * A007334  Number of spanning trees in the graph K_{n}/e, which results from
 *          contracting an edge e in the complete graph K_{n} on n vertices
 *          (for n>=2).
 * https://oeis.org/A007334
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2*n^(n-3) for n>=3, a(2) = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 2) {
        return 1;
    }
    seq_t base = (seq_t)n;
    seq_t result = 2;
    idx_t e = n - 3;
    while (e > 0) {
        if (e & 1) {
            result *= base;
        }
        e >>= 1;
        if (e > 0) {
            base *= base;
        }
    }
    return result;
}
