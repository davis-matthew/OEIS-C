/*
 * A015400  Inverse of 1391st cyclotomic polynomial.
 * https://oeis.org/A015400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 1391 ∈ [0..12], * a(n) = -1 if n mod 1391 ∈
 *            [107..118], * otherwise a(n) = 0. Period 1391
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t period = 1391ULL;
    idx_t m = n % period;

    if (m < 13ULL)
        return (seq_t)1;
    if (m <= 106ULL)
        return (seq_t)0;
    if (m <= 118ULL)
        return (seq_t)-1;
    return (seq_t)0;
}
