/*
 * A040422  Continued fraction for sqrt(444).
 * https://oeis.org/A040422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0)=21, a(1)=14, a(n)=a(n-2) for n≥2
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A040422 */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)21;
    if (n & 1)  return (seq_t)14;   /* odd index */
    return (seq_t)42;                /* even index ≥2 */
}
