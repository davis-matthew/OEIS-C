/*
 * A240354  Inverse of 71st cyclotomic polynomial.
 * https://oeis.org/A240354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A240354: Inverse of the 71st cyclotomic polynomial.
   For prime p=71, Φ_p(x) = 1 + x + ... + x^(p-1).
   Its reciprocal is (1 - x)/(1 - x^p) = Σ_{m≥0} (x^(mp) - x^(mp+1)).
   Hence a(n) = 1 if n ≡ 0 (mod 71),
               -1 if n ≡ 1 (mod 71),
                0 otherwise.
   Formula: a(n) = (n mod 71 == 0) ? 1 : (n mod 71 == 1) ? -1 : 0.
*/

seq_t a(idx_t n) {
    const idx_t P = 71;
    idx_t r = n % P;
    if (r == 0) return (seq_t)1;
    if (r == 1) return (seq_t)-1;
    return (seq_t)0;
}
