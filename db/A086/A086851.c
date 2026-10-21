/*
 * A086851  a(0) = 1, a(n+1) = a(n)^2 - n.
 * https://oeis.org/A086851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1; a(n+1) = a(n)^2 - n
 * Range:     i8 n <= 8, i16 n <= 9, i32 n <= 10, i64 n <= 11, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th term of OEIS sequence A086851.
   Works for any seq_t (integer or arbitrary‑precision class) and
   any idx_t (unsigned 64 bit). */
seq_t a(idx_t n)
{
    seq_t acc = (seq_t)1;          /* a(0) */
    for (idx_t i = 0; i < n; ++i) {
        acc = acc * acc - (seq_t)i; /* a(i+1) = a(i)^2 - i */
    }
    return acc;
}
