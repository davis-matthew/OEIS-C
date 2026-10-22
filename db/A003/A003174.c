/*
 * A003174  Positive squarefree integers D such that Q[sqrt(D)] is a quadratic
 *          field which is norm-Euclidean.
 * https://oeis.org/A003174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full, nice
 * Formula:   a(n) returns the n-th positive squarefree D for which Q[sqrt(D)]
 *            is norm-Euclidean. Complete finite list: 2, 3, 5, 6, 7, 11, 13,
 *            17, 19, 21, 29, 33, 37, 41, 57, 73
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t values[] = {2, 3, 5, 6, 7, 11, 13, 17, 19, 21, 29, 33, 37, 41, 57, 73};
    static const idx_t count = 16;
    
    if (n >= 1 && n <= count) {
        return values[n - 1];
    }
    return 0;
}
