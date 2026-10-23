/*
 * A048696  Generalized Pellian with second term equal to 9.
 * https://oeis.org/A048696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2); a(0) = 1, a(1) = 9
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 23, u64 n <= 48, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Direct computation using the recurrence. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;

    seq_t a0 = (seq_t)1;          /* a(0) */
    seq_t a1 = (seq_t)9;          /* a(1) */
    seq_t an;

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        an = (seq_t)2 * a1 + a0;  /* a(i) = 2*a(i-1) + a(i-2) */
        a0 = a1;                 /* shift for next iteration */
        a1 = an;
    }
    return a1;                    /* a(n) */
}
