/*
 * A133480  Left 3-step factorial (n,-3)!: a(n) = (-1)^n * A008544(n).
 * https://oeis.org/A133480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1, a(n) = a(n-1) * (1 - 3*n) for n ≥ 1 (a(0)=1)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 15, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to  a(n) = (-1)^n * A008544(n)  (OEIS A133480) */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t factor = (seq_t)1 - ((seq_t)3 * (seq_t)i);
        res *= factor;
    }
    return res;
}
