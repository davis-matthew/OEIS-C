/*
 * A176333  Expansion of (1-3*x)/(1-4*x+9*x^2).
 * https://oeis.org/A176333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 4*a(n-1) - 9*a(n-2), a(0)=1, a(1)=1
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 20, i64 n <= 39, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses an O(n) iterative computation and
   requires no dynamic memory, making it compatible with
   arbitrary integer types (primitive or arbitrary-precision). */
seq_t a(idx_t n)
{
    /* Return the first two terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* Iterate using two running values. */
    seq_t a0 = (seq_t)1;   /* a(n-2) */
    seq_t a1 = (seq_t)1;   /* a(n-1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        an = ((seq_t)4) * a1 - ((seq_t)9) * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
