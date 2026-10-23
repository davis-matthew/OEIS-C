/*
 * A016192  Inverse of 2183rd cyclotomic polynomial.
 * https://oeis.org/A016192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, changed
 * Formula:   a(n) = 1 if n mod 2183 ∈ [0,36], -1 if n mod 2183 ∈ [59,95], 0
 *            otherwise
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t period = 2183;
    idx_t r = n % period;
    if (r <= 36) {
        return (seq_t)1;
    }
    if (r >= 59 && r <= 95) {
        return (seq_t)(-1);
    }
    return (seq_t)0;
}
