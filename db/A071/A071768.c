/*
 * A071768  Determinant of the n X n matrix whose element (i,j) equals |i-j|
 *          (mod 3).
 * https://oeis.org/A071768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1; for n>0 if n ≡ 0 (mod 3) : a(n) = 4 if n ≡ 1 (mod 3) :
 *            a(n) = 9*(n-1)/3 if n ≡ 2 (mod 3) : a(n) = -9*(n-2)/3 - 1 (The
 *            recurrence a(n) = -a(n-1)-a(n-2)+a(n-3)+a(n-4)+a(n-5) holds for
 *            n≥6.)
 * Range:     i8 n <= 45, i16 n <= 64, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    /* n is unsigned, but modulo and arithmetic work in idx_t */
    switch (n % 3) {
        case 0:    /* n ≡ 0 (mod 3) */
            return (seq_t)4;
        case 1:    /* n ≡ 1 (mod 3) */
            return ((seq_t)9 * ( (seq_t)n - 1 )) / 3;
        case 2:    /* n ≡ 2 (mod 3) */
            return -(((seq_t)9 * ( (seq_t)n - 2 )) / 3) - 1;
    }
    /* Unreachable, but keep compiler happy */
    return (seq_t)0;
}
