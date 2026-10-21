/*
 * A225003  Duplicate of A047332.
 * https://oeis.org/A225003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 23 if n == 1 a(n) = n + ⌊2n/5⌋ – 1 if n % 5 ∈ {0,1,3}
 *            a(n) = n + ⌊2n/5⌋ otherwise
 * Range:     i8 n <= 67, i16 n <= 67, i32 n <= 67, i64 n <= 67, i128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) {
        return 23;
    }
    seq_t nn = (seq_t)n;                 /* convert index for arithmetic */
    idx_t r = n % 5;                    /* n modulo 5 (circular) */
    seq_t diff = (nn * 2) / 5;          /* floor(2n/5) */
    if (r == 0 || r == 1 || r == 3) {
        diff -= 1;
    }
    return nn + diff;                   /* final result */
}
