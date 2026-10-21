/*
 * A063243  Duplicate of A063232.
 * https://oeis.org/A063243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 13, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 5 for n = 1,
 *              a(n) = 10*n - 4  if n is even (n ≥ 2),
 *              a(n) = 10*n - 6  if n is odd  (n ≥ 3)
 */
seq_t a(idx_t n)
{
    if (n == 1) return 5;
    if ((n & 1) == 0) {          /* even n */
        return ((seq_t)n * 10) - 4;
    } else {                     /* odd n */
        return ((seq_t)n * 10) - 6;
    }
}
