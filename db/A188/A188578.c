/*
 * A188578  Expansion of (1 - x^3) * (1 - x^5) * (1 - x^6) / (1 - x^15) in
 *          powers of x.
 * https://oeis.org/A188578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficients of the numerator (1 - x^3 - x^5 - x^6 + x^8
 *            + x^9 + x^11 - x^14) repeated every 15, i.e. a(n) = coefficient
 *            at index n mod 15 in the generating function expansion
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t coef[15] = {
        (seq_t)1,   /* n ≡ 0 (mod 15) */
        0,          /* n ≡ 1 (mod 15) */
        0,          /* n ≡ 2 (mod 15) */
        (seq_t)-1,  /* n ≡ 3 (mod 15) */
        0,          /* n ≡ 4 (mod 15) */
        (seq_t)-1,  /* n ≡ 5 (mod 15) */
        (seq_t)-1,  /* n ≡ 6 (mod 15) */
        0,          /* n ≡ 7 (mod 15) */
        (seq_t)1,   /* n ≡ 8 (mod 15) */
        (seq_t)1,   /* n ≡ 9 (mod 15) */
        0,          /* n ≡10 (mod 15) */
        (seq_t)1,   /* n ≡11 (mod 15) */
        0,          /* n ≡12 (mod 15) */
        0,          /* n ≡13 (mod 15) */
        (seq_t)-1   /* n ≡14 (mod 15) */
    };
    return coef[n % (idx_t)15];
}
