/*
 * A219012  Numerators in a product expansion for sqrt(3).
 * https://oeis.org/A219012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = a(n)^5 - 5*a(n)^3 + 5*a(n), a(0) = 4
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t a_n = 4;                 /* a(0) = 4 */
    for (idx_t i = 0; i < n; ++i) {
        seq_t a2 = a_n * a_n;                                /* a^2   */
        seq_t a3 = a2 * a_n;                                 /* a^3   */
        seq_t a5 = a3 * a2;                                 /* a^5   */
        a_n = a5 - 5 * a3 + 5 * a_n;                        /* recurrence */
    }
    return a_n;                       /* returns a(n) */
}
