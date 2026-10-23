/*
 * A015550  Inverse of 1541st cyclotomic polynomial.
 * https://oeis.org/A015550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 if n mod 1541 ∈ [0,22], * 0 if n mod 1541 ∈ [23,66] or
 *            ∈ [90,1540], * -1 if n mod 1541 ∈ [67,89]
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use the period 1541 */
    idx_t r = n % 1541U;

    if (r < 23U)
        return (seq_t)1;
    if (r < 67U)
        return (seq_t)0;
    if (r < 90U)
        return (seq_t)-1;
    return (seq_t)0;
}
