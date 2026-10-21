/*
 * A089658  a(n) = S1(n,1), where S1(n, t) = Sum_{k=0..n} (k^t * Sum_{j=0..k}
 *          binomial(n,j)).
 * https://oeis.org/A089658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(5 + 3*n) * 2^(n-3) (integer division is safe)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 24, u64 n <= 53, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute the common factor n*(5+3*n). */
    seq_t base = (seq_t)n * (((seq_t)n * (seq_t)3) + (seq_t)5);
    if (n >= 3) {
        /* Shift left by (n-3).  Shift amount must be an unsigned integer. */
        return base << (unsigned long)(n - 3);
    } else {
        /* Shift right by (3-n).  The shift amount is positive. */
        return base >> (unsigned long)(3 - n);
    }
}
