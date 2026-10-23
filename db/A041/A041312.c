/*
 * A041312  Numerators of continued fraction convergents to sqrt(170).
 * https://oeis.org/A041312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 26*a(n-1) + a(n-2)   (OEIS A041312)
   Initial values: a(0) = 13, a(1) = 339
   Computes the numerator of the nth convergent to sqrt(170). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)13;
    if (n == 1) return (seq_t)339;
    seq_t a0 = 13;
    seq_t a1 = 339;
    seq_t an;
    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)26 * a1 + a0;
        a0  = a1;
        a1  = an;
    }
    return an;
}
