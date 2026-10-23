/*
 * A014350  Inverse of 341st cyclotomic polynomial.
 * https://oeis.org/A014350
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

/* Sequence A014350: periodic with period 341.
   a(n) =  1  if 0 ≤ n mod 341 ≤ 10
          -1  if 31 ≤ n mod 341 ≤ 41
           0  otherwise */
seq_t a(idx_t n) {
    idx_t r = n % 341ULL;          /* 341 fits in 64‑bit */
    if (r <= 10ULL)
        return (seq_t)1;
    if (r >= 31ULL && r <= 41ULL)
        return (seq_t)(-1);
    return (seq_t)0;
}
