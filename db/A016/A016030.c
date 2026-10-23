/*
 * A016030  Inverse of 2021st cyclotomic polynomial.
 * https://oeis.org/A016030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 2021 ∈ [0,42]; * a(n) = -1 if n mod 2021 ∈
 *            [47,89]; * a(n) = 0 otherwise. * The sequence is periodic with
 *            period 2021
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const idx_t period = 2021;
    idx_t r = n % period;
    seq_t val;
    if (r <= 42)           val = (seq_t)1;
    else if (r <= 46)      val = (seq_t)0;
    else if (r <= 89)      val = (seq_t)-1;
    else                   val = (seq_t)0;
    return val;
}
