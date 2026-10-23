/*
 * A100062  Denominator of the probability that an integer n occurs in the
 *          cumulative sums of the decimal digits of a random real number
 *          between 0 and 1.
 * https://oeis.org/A100062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, frac
 * Formula:   a(n) = 9^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n >= 1, a(n) = 9^n; for n = 0 the behavior is undefined but
       we return 1 (the neutral element of multiplication) which is
       harmless for seq_t values that wrap naturally. */
    seq_t result = 1;
    for (idx_t i = 0; i < n; ++i)
        result *= (seq_t)9;
    return result;
}
