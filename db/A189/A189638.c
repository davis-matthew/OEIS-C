/*
 * A189638  Partial sums of A116178.
 * https://oeis.org/A189638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(floor((n+1)/3)) + floor(n/3), a(0) = 0 */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    return a((n + 1) / 3) + (seq_t)(n / 3);
}
