/*
 * A014128  Inverse of 119th cyclotomic polynomial.
 * https://oeis.org/A014128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n ≡ 0,…,6 (mod 119), a(n) = –1 if n ≡ 17,…,23 (mod
 *            119), a(n) = 0 otherwise
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 119u;   /* remainder modulo 119 */
    if (r <= 6u)          /* 0 ≤ r ≤ 6  →  +1 */
        return 1;
    if (r >= 17u && r <= 23u)   /* 17 ≤ r ≤ 23 → –1 */
        return -1;
    return 0;            /* all other residues  →  0 */
}
