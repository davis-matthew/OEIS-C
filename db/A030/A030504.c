/*
 * A030504  Graham-Sloane-type lower bound on the size of a ternary (n,3,4)
 *          constant-weight code.
 * https://oeis.org/A030504
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil( binomial(n,4) * 16 / (2*n + 1) )
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset is 4: values for n < 4 are irrelevant, return 0 */
    if (n < 4)
        return (seq_t)0;

    /* binomial(n,4) = n*(n-1)*(n-2)*(n-3)/24   (exact integer division) */
    seq_t bin = (seq_t)n * (n - 1) * (n - 2) * (n - 3) / 24;

    /* numerator and denominator for the fraction */
    seq_t num = bin * 16;
    seq_t den = (seq_t)n * 2 + 1;            /* (2*n + 1) */

    seq_t res = num / den;                   /* floor division */
    if (num % den != 0)                      /* add one if fraction exists */
        res += 1;

    return res;
}
