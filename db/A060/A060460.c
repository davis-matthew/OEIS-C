/*
 * A060460  Consider the final n decimal digits of 2^j for all values of j.
 *          They are periodic. Sequence gives position (or phase) of the
 *          maximal value seen in these n digits.
 * https://oeis.org/A060460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 2*5^(n-1) + n (offset 1, a(1)=3)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, but allow n==0 return 0 */
        return 0;

    /* Compute 5^(n-1) using exponentiation by squaring */
    seq_t pow5 = 1;
    seq_t base  = (seq_t)5;
    idx_t e = n - 1;          /* exponent is n-1 */

    while (e > 0) {
        if (e & 1)
            pow5 *= base;    /* multiply result by current base when bit is set */
        base *= base;        /* square the base for the next bit */
        e >>= 1;
    }

    /* Return 2*5^(n-1) + n */
    return pow5 * (seq_t)2 + (seq_t)n;
}
