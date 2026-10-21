/*
 * A079003  Least k >= 3 such that Fibonacci(k) == -1 (mod 3^n).
 * https://oeis.org/A079003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 3, a(n) = 4*3^(n-2) + 2 for n >= 2 (which is equivalent
 *            to a(n) = 3*a(n-1) - 4 for n > 1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)3;        /* a(1) = 3 */

    /* Compute 3^(n-2) using binary exponentiation */
    idx_t exp = n - 2;
    seq_t power = (seq_t)1;
    seq_t base  = (seq_t)3;

    while (exp)
    {
        if (exp & 1U)
            power *= base;     /* multiply when bit is set */
        base *= base;          /* square the base */
        exp >>= 1U;            /* shift exponent right */
    }

    /* a(n) = 4*power + 2 */
    seq_t result = power * (seq_t)4;
    result += (seq_t)2;
    return result;
}
