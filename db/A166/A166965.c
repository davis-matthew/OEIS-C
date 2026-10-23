/*
 * A166965  a(n) = 20*a(n-1) - 64*a(n-2) for n > 1; a(0) = 1, a(1) = 19.
 * https://oeis.org/A166965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 20*a(n-1) - 64*a(n-2), a(0)=1, a(1)=19 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)19;
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)19;
    seq_t an;
    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)20 * a1 - (seq_t)64 * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
