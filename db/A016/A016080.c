/*
 * A016080  Inverse of 2071st cyclotomic polynomial.
 * https://oeis.org/A016080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 for 0 ≤ n mod 2071 < 19, 0 for 19 ≤ n mod 2071 < 109,
 *            -1 for 109 ≤ n mod 2071 < 128, 0 for 128 ≤ n mod 2071 < 2071
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic with period 2071.  */
seq_t a(idx_t n)
{
    static const idx_t N = 2071ULL;
    idx_t r = n % N;

    if (r < 19)          return (seq_t)1;
    if (r < 109)         return (seq_t)0;
    if (r < 128)         return (seq_t)-1;
    return (seq_t)0;
}
