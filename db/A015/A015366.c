/*
 * A015366  Inverse of 1357th cyclotomic polynomial.
 * https://oeis.org/A015366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 1357 ∈ [0,22]; 0 if n mod 1357 ∈ [23,58] or
 *            [82,1356]; -1 if n mod 1357 ∈ [59,81]
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* index modulo the period */
    idx_t r = n % 1357UL;
    /* 1 for 0..22 */
    if (r <= 22ULL) return (seq_t)1;
    /* 0 for 23..58 */
    if (r <= 58ULL) return (seq_t)0;
    /* -1 for 59..81 */
    if (r <= 81ULL) return (seq_t)-1;
    /* 0 for 82..1356 */
    return (seq_t)0;
}
