/*
 * A058200  Coefficients of the highest power of r in a sequence of parametric
 *          solutions for the Diophantine equation x^3+y^3+z^3=1.
 * https://oeis.org/A058200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 9, a(n) = 9 * 432^(n-1) for n > 0
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* The first term in the list */
        return (seq_t)9;
    }

    /* Fast exponentiation: compute 432^(n-1) */
    idx_t exp   = n - 1;      /* number of times to multiply by 432 */
    seq_t base  = (seq_t)432;
    seq_t result = 1;        /* accumulator for the power */

    while (exp) {
        if (exp & 1) {
            result *= base;
        }
        exp >>= 1;
        if (exp) {
            base *= base;   /* square the base for the next bit */
        }
    }

    /* Multiply by the leading 9 to obtain a(n) */
    return result * (seq_t)9;
}
