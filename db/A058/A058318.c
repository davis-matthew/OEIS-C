/*
 * A058318  Number of energy levels in atoms of the n-th element of the
 *          periodic table.
 * https://oeis.org/A058318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = m for s(m-1) < n ≤ s(m), where s = [0, 2, 10, 18, 36, 54,
 *            86, 118] (cumulative run lengths) (for n > 118 the sequence is
 *            undefined; we return the last value 7)
 * Range:     u8 n <= 118, u16 n <= 118, u32 n <= 118, u64 n <= 118,
 *            u128 n <= 118, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* cummulative bounds for energy‑level counts 1..7 */
    static const idx_t bounds[] = {0, 2, 10, 18, 36, 54, 86, 118};
    for (idx_t m = 1; m <= 7; ++m) {
        if (n <= bounds[m]) {
            return (seq_t)m;
        }
    }
    /* If n exceeds known limits, default to the maximum energy level observed. */
    return (seq_t)7;
}
