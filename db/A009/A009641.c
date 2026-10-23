/*
 * A009641  a(n) = Product_{i=0..6} floor((n+i)/7).
 * https://oeis.org/A009641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{i=0..6} floor((n+i)/7)
 * Range:     u8 n <= 15, u16 n <= 34, u32 n <= 166, u64 n <= 3957,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* For n < 7, some factor is 0, so result is 0 */
    seq_t prod = 1;
    for (idx_t i = 0; i < 7; ++i) {
        seq_t term = (seq_t)(n + i) / 7;   /* floor division */
        if (term == 0) return 0;
        prod *= term;
    }
    return prod;
}
