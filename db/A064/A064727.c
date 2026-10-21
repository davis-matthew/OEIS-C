/*
 * A064727  Number of pairs x,y such that 0 < x <= y < n and x+y = n and x*y =
 *          k*n for some k.
 * https://oeis.org/A064727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = |{ x | 1 ≤ x ≤ ⌊n/2⌋ , n | x*(n−x) }| a(1) = 0; the
 *            function is defined for n ≥ 1
 * Range:     u8 n <= 35, u16 n <= 4850, u32 n <= 9831, u64 n <= 9831,
 *            u128 n <= 9831, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t count = 0;
    if (n < 2) return 0;              /* n = 1 gives 0 */
    seq_t sn = (seq_t)n;

    idx_t half = n >> 1;              /* floor(n/2) */
    for (idx_t x = 1; x <= half; ++x) {
        seq_t prod = (seq_t)x * (seq_t)(n - x);
        if (prod % sn == 0) ++count;
    }
    return count;
}
